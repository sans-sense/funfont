/*
  original code from http://pastebin.com/0dV84hBa
  g++ -ltesseract -o fontRecog fontRecog.cpp -llept -lz -lpng -ljpeg -ltiff
  g++ -Wall -I ../mylib/lept-bin/include/ -L../mylib/lept-bin/lib/ -ltesseract -o fontRecog fontRecog.cpp -llept -lz -lpng -ljpeg -ltiff && ./fontRecog ../data/images/50a89cf310daa503e4000011_large.jpg
*/
 
#include <tesseract/baseapi.h>
#include <leptonica/allheaders.h>
#include <string.h>
#include <ctype.h>

struct Score{
  int value;
  unsigned int fontType;
  const char* word;
  Score* next;
};

// between sans and serif, serif wins
enum FontType {
  SANS_SERIF   = (1u << 0),
  SERIF        = (1u << 1),
  HAND_WRITING = (1u << 2),
  MONO_SPACE   = (1u << 3)
};

bool isNearlyAlphaNumeric(const char* word);
Score* computeScore(unsigned int fontType, const char* word);
void printFontSummary(Score* head, int scoreCount);

bool log_debug = false;
 
int main(int argc, char* argv[]) {
    tesseract::TessBaseAPI *api = new tesseract::TessBaseAPI();
 
    if (api->Init(NULL, "eng")) {
        fprintf(stderr, "Could not initialize tesseract.\n");
        exit(1);
    }
 
    Pix *pix = pixRead(argv[1]);
    api->SetImage(pix);
 
    printf("\n Analyzing image %s \n", argv[1]);

    int lcount = 1;
    int scoreCount = 0;
    api->Recognize(0);
    tesseract::ResultIterator* ri = api->GetIterator();
    Score *score = NULL;
    Score *head = NULL;

    if (ri != 0) {
        do {
            const char* word = ri->GetUTF8Text(tesseract::RIL_WORD);
            if (word != 0) {
                bool bold, italic, underlined, monospace, serif, smallcaps;
                int point_size, font_id;
                unsigned int fontType;

                ri->WordFontAttributes(&bold, &italic, &underlined,
                                                   &monospace, &serif,
                                                   &smallcaps, &point_size,
                                                   &font_id);
                fontType = (serif)? SERIF:SANS_SERIF;
                
                if (monospace) {
                  fontType = fontType | MONO_SPACE;
                }

                if (log_debug) {
                  printf("%s \t", word);
                }

                if (isNearlyAlphaNumeric(word)) {
                  Score* next_score = computeScore(fontType, word);

                  if (score) {
                    score->next = next_score;
                  } else {
                    head = next_score;
                  }
                  score = next_score;
                  scoreCount++;
                }
            } else {
              delete[] word;
            }
            lcount++;
        } while (ri->Next(tesseract::RIL_WORD));

        // based on score create font type summary
        printFontSummary(head, scoreCount);
    }
 
    delete ri;
    api->End();
    pixDestroy(&pix);
    return 0;
}

bool isNearlyAlphaNumeric(const char* word) {
  // iterate through and if aplha numeric chars constitute more than 80%, return true else false;
  int alphaCount = 0;
  for (int i = 0; i < strlen(word); i++) {
    if (isascii(word[i])) {
      alphaCount++;
    }
  }
  return ((alphaCount/strlen(word)) > 0.8) && (strlen(word) > 2);
}

Score* computeScore(unsigned int fontType, const char* word) {
  Score* score = (Score*)malloc(sizeof(Score));
  score->value = strlen(word);
  score->fontType = fontType;
  score->word = word;
  return score;
}

void printFontSummary(Score* head, int scoreCount) {
  int i = 0;
  Score* prevScore;
  Score* score;
  char content[1024];
  char fontSummary[1024];
  unsigned int totalFontType = 0;

  if (head) {
    strcpy(content, "");
    strcpy(fontSummary, "");
    score = head;
    do {
      if (log_debug) {
        printf("%d %u %s \t", score->value, score->fontType, score->word);
      }
      if (strlen(content) < 1024) {
        strncat(content, score->word, strlen(score->word));
        strncat(content, " ", 1);
      }

      totalFontType = totalFontType | score->fontType;
      prevScore = score;
      score = score->next;
            
      // cleanup
      delete[] prevScore->word;
      delete prevScore;
      i++;
    } while(i < scoreCount);
    if (totalFontType & SANS_SERIF) {
      strncat(fontSummary, "sans-serif ", 11);
    }
    if (totalFontType & SERIF) {
      strncat(fontSummary, "serif ", 7);
    }
    if (totalFontType & MONO_SPACE) {
      strncat(fontSummary, "mono-space ", 11);
    }

  } else {
    strcpy(fontSummary, "hand-written");
    strcpy(content, "unrecognized");
  }
  printf("content -> %s, classification -> %s  \n", content, fontSummary);

}
