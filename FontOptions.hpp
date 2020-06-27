#ifndef FONT_OPTIONS_H
#define FONT_OPTIONS_H

#define TEXT_ALIGN_LEFT 1
#define TEXT_ALIGN_CENTER 2
#define TEXT_ALIGN_RIGHT 3

class FontOptions {
public:
  const GFXfont* font;
  uint8_t fontSize;
  uint8_t fontColor;
  char textAlign;

public:
  FontOptions(const GFXfont *font = NULL, uint8_t size = 1, uint16_t color = WHITE, char textAlign = TEXT_ALIGN_LEFT) {
    this->font = font;
    this->fontSize = size;
    this->fontColor = color;
    this->textAlign = textAlign;
  }
};

#endif
