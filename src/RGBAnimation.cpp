#include "RGBAnimation.h"  
#include "RGBAnimationPath.h"

RGBAnimationClass::RGBAnimationClass(RGBmatrixPanel & display) 
  : RGBAnimationClass(display, 0, 0, display.width(), display.height(), -1, 0) 
{ };
RGBAnimationClass::RGBAnimationClass(RGBmatrixPanel & display, int16_t x, int16_t y, int16_t w, int16_t h, int16_t frames, int32_t backColor) 
  : _d(display), _pos(POINT(x,y)), _size(POINT(w,h)), _frames(frames), _backColor(backColor), _curFrame(0)
{ }

int16_t RGBAnimationClass::drawFrame(int16_t frame)
{
  if (frame >= 0)
    _curFrame = frame;
  if (_frames > 0)
    _curFrame %= _frames;

  for (std::vector<RGBAnimationPathClass*>::iterator path = _paths.begin() ; path != _paths.end(); path++)
  {
    Serial.println("StartAnimationFrame");
    (*path)->animate();
    Serial.println("EndAnimationFrame");
  }  

  draw();
  
  if (frame < 0)
    _curFrame++;
  if (_frames > 0)
    _curFrame %= _frames;

  return _curFrame;
}

POINT RGBAnimationClass::moveTo(int16_t x, int16_t y, bool clearFirst)
{
  if ((getCaps() & CAN_MOVE) == CAN_MOVE)
  {
    if (clearFirst) cls();
    _pos.x = constrain(x, -_size.x, _d.width() );
    _pos.y = constrain(y, -_size.y, _d.height());
  }
  return _pos;
}

POINT RGBAnimationClass::move(int16_t dx, int16_t dy, bool clearFirst)
{
  if ((getCaps() & CAN_MOVE) == CAN_MOVE)
  {
    if (clearFirst) cls();
    _pos.x = constrain(_pos.x + dx, -_size.x, _d.width() );
    _pos.y = constrain(_pos.y + dy, -_size.y, _d.height());
  }
  return _pos;
}

POINT RGBAnimationClass::setSize(uint16_t w, uint16_t h, bool clearFirst)
{
  if ((getCaps() & CAN_RESIZE) == CAN_RESIZE)
  {
    if (clearFirst) cls();
    if (w > 0)
      _size.x = w;
    if (h > 0)    
      _size.y = h;
  }      
  return _size;
}

POINT RGBAnimationClass::resize(int16_t dw, int16_t dh, bool clearFirst)
{
  if ((getCaps() & CAN_RESIZE) == CAN_RESIZE)
  {
    if (clearFirst) cls();
    if (_size.x + dw > 0)
      _size.x += dw;
    if (_size.y + dh > 0)    
      _size.y += dh;
  }      
  return _size;
}

void RGBAnimationClass::draw()
{
  cls();
  _d.drawLine(_pos.x, _pos.y, _pos.x + _size.x, _pos.y + _size.y, ~_backColor);
  _d.drawLine(_pos.x, _pos.y + _size.y, _pos.x + _size.x, _pos.y, ~_backColor);
}

void RGBAnimationClass::cls()
{
  if(_backColor >= 0)
    _d.fillRect(_pos.x, _pos.y, _size.x,_size.y, _backColor);
}

RGBAnimationPathClass* RGBAnimationClass::add(RGBAnimationPathClass* path)
{
  _paths.push_back(path);
  RGBAnimationPathClass* newPath = _paths.back();
  if (!newPath->_owner) newPath->_owner = this;
  return newPath;
}

void RGBAnimationClass::print3x6(int16_t x, int16_t y, const char* text, uint16_t color, int32_t backColor)
{
    int len = strlen(text);
    
    if (backColor >= 0)
      _d.fillRect(x, y, 3*len, 6, backColor);
      
    for (int i = 0; i < len; i++)
    {
      if (text[i] <= '~')
      {
        _d.drawBitmap(x + i*3, y, (uint8_t*)&font3x6[(uint8_t)text[i]], 8, 6, color);
      }
    }
}

 // (since bitmaps start on byte boundaries the extra five pixels appended to fill byte)
const uint8_t RGBAnimationClass::font3x6[][6] PROGMEM = { 
    {0,0,0,0,0,0},		
    {// �
     0b01100000,
     0b01100000,
     0b00000000,
     0b00000000,
     0b00000000,
     0b00000000,
    },
    {0,0,0,0,0,0},		
    {0,0,0,0,0,0},		
    {0,0,0,0,0,0},		
    {0,0,0,0,0,0},		
    {0,0,0,0,0,0},		
    {0,0,0,0,0,0},		
    {0,0,0,0,0,0},		
    {0,0,0,0,0,0},		
    {0,0,0,0,0,0},		
    {0,0,0,0,0,0},		
    {0,0,0,0,0,0},		
    {0,0,0,0,0,0},		
    {0,0,0,0,0,0},		
    {0,0,0,0,0,0},		
    {0,0,0,0,0,0},		
    {0,0,0,0,0,0},		
    {0,0,0,0,0,0},		
    {0,0,0,0,0,0},		
    {0,0,0,0,0,0},		
    {0,0,0,0,0,0},		
    {0,0,0,0,0,0},		
    {0,0,0,0,0,0},		
    {0,0,0,0,0,0},		
    {0,0,0,0,0,0},		
    {0,0,0,0,0,0},		
    {0,0,0,0,0,0},		
    {0,0,0,0,0,0},		
    {0,0,0,0,0,0},		
    {0,0,0,0,0,0},		
    {0,0,0,0,0,0},		
    {0,0,0,0,0,0}, // blank
    {// !
     0b01000000,
     0b01000000,
     0b01000000,
     0b00000000,
     0b01000000,
     0b00000000,
    }, 
    {// "
     0b10100000,
     0b10100000,
     0b00000000,
     0b00000000,
     0b00000000,
     0b00000000,
    }, 
    {// #
     0b10000000,
     0b10100000,
     0b11100000,
     0b11100000,
     0b10100000,
     0b00100000,
    }, 
    {// $
     0b01100000,
     0b11100000,
     0b01000000,
     0b11100000,
     0b11000000,
     0b00000000,
    }, 
    {// %
     0b10100000,
     0b00100000,
     0b01000000,
     0b10000000,
     0b10100000,
     0b00000000,
    }, 
    {// &
     0b01000000,
     0b10100000,
     0b01000000,
     0b10100000,
     0b01100000,
     0b00000000,
    }, 
    {// '
     0b01000000,
     0b01000000,
     0b00000000,
     0b00000000,
     0b00000000,
     0b00000000,
    }, 
    {// (
     0b01000000,
     0b10000000,
     0b10000000,
     0b10000000,
     0b01000000,
     0b00000000,
    }, 
    {// )
     0b01000000,
     0b00100000,
     0b00100000,
     0b00100000,
     0b01000000,
     0b00000000,
    }, 
    {// *
     0b00000000,
     0b10100000,
     0b01000000,
     0b10100000,
     0b00000000,
     0b00000000,
    }, 
    {// +
     0b00000000,
     0b01000000,
     0b11100000,
     0b01000000,
     0b00000000,
     0b00000000,
    }, 
    {// ,
     0b00000000,
     0b00000000,
     0b00000000,
     0b01000000,
     0b10000000,
     0b00000000,
    }, 
    {// -
     0b00000000,
     0b00000000,
     0b11100000,
     0b00000000,
     0b00000000,
     0b00000000,
    }, 
    {// .
     0b00000000,
     0b00000000,
     0b00000000,
     0b00000000,
     0b01000000,
     0b00000000,
    }, 
    {// /
     0b00100000,
     0b00000000,
     0b01000000,
     0b00000000,
     0b10000000,
     0b00000000,
    }, 
    {// 0
     0b01000000,
     0b10100000,
     0b11100000,
     0b10100000,
     0b01000000,
     0b00000000,
    }, 
    {// 1
     0b01000000,
     0b11000000,
     0b01000000,
     0b01000000,
     0b11100000,
     0b00000000,
    }, 
    {// 2
     0b11000000,
     0b00100000,
     0b01000000,
     0b10000000,
     0b11100000,
     0b00000000,
    }, 
    {// 3
     0b11100000,
     0b00100000,
     0b11000000,
     0b00100000,
     0b11000000,
     0b00000000,
    }, 
    {// 4
     0b10100000,
     0b10100000,
     0b11100000,
     0b00100000,
     0b00100000,
     0b00000000,
    }, 
    {// 5
     0b11100000,
     0b10000000,
     0b11000000,
     0b00100000,
     0b11000000,
     0b00000000,
    }, 
    {// 6
     0b01100000,
     0b10000000,
     0b11100000,
     0b10100000,
     0b11100000,
     0b00000000,
    }, 
    {// 7
     0b11100000,
     0b00100000,
     0b00100000,
     0b01000000,
     0b01000000,
     0b00000000,
    }, 
    {// 8
     0b11100000,
     0b10100000,
     0b01000000,
     0b10100000,
     0b11100000,
     0b00000000,
    }, 
    {// 9
     0b11100000,
     0b10100000,
     0b11100000,
     0b00100000,
     0b11000000,
     0b00000000,
    }, 
    {// :
     0b00000000,
     0b01000000,
     0b00000000,
     0b01000000,
     0b00000000,
     0b00000000,
    }, 
    {// ;
     0b00000000,
     0b01000000,
     0b00000000,
     0b01000000,
     0b10000000,
     0b00000000,
    }, 
    {// <
     0b00100000,
     0b01000000,
     0b10000000,
     0b01000000,
     0b00100000,
     0b00000000,
    }, 
    {// =
     0b00000000,
     0b11100000,
     0b00000000,
     0b11100000,
     0b00000000,
     0b00000000,
    }, 
    {// >
     0b10000000,
     0b01000000,
     0b00100000,
     0b01000000,
     0b10000000,
     0b00000000,
    }, 
    {// ?
     0b11000000,
     0b00100000,
     0b01000000,
     0b00000000,
     0b01000000,
     0b00000000,
    }, 
    {// @
     0b11100000,
     0b10100000,
     0b10100000,
     0b10000000,
     0b01100000,
     0b00000000,
    }, 
    {// A
     0b01000000,
     0b10100000,
     0b11100000,
     0b10100000,
     0b10100000,
     0b00000000,
    }, 
    {// B
     0b11000000,
     0b10100000,
     0b11000000,
     0b10100000,
     0b11000000,
     0b00000000,
    }, 
    {// C
     0b01100000,
     0b10000000,
     0b10000000,
     0b10000000,
     0b01100000,
     0b00000000,
    }, 
    {// D
     0b11000000,
     0b10100000,
     0b10100000,
     0b10100000,
     0b11000000,
     0b00000000,
    }, 
    {// E
     0b11100000,
     0b10000000,
     0b11000000,
     0b10000000,
     0b11100000,
     0b00000000,
    }, 
    {// F
     0b11100000,
     0b10000000,
     0b11000000,
     0b10000000,
     0b10000000,
     0b00000000,
    }, 
    {// G
     0b01100000,
     0b10000000,
     0b10100000,
     0b10100000,
     0b01100000,
     0b00000000,
    }, 
    {// H
     0b10100000,
     0b10100000,
     0b11100000,
     0b10100000,
     0b10100000,
     0b00000000,
    }, 
    {// I
     0b11100000,
     0b01000000,
     0b01000000,
     0b01000000,
     0b11100000,
     0b00000000,
    }, 
    {// J
     0b11100000,
     0b00100000,
     0b00100000,
     0b10100000,
     0b01000000,
     0b00000000,
    }, 
    {// K
     0b10100000,
     0b10100000,
     0b11000000,
     0b10100000,
     0b10100000,
     0b00000000,
    }, 
    {// L
     0b10000000,
     0b10000000,
     0b10000000,
     0b10000000,
     0b11100000,
     0b00000000,
    }, 
    {// M
     0b10100000,
     0b11100000,
     0b11100000,
     0b10100000,
     0b10100000,
     0b00000000,
    }, 
    {// N
     0b10100000,
     0b11100000,
     0b11100000,
     0b11100000,
     0b10100000,
     0b00000000,
    }, 
    {// O
     0b01000000,
     0b10100000,
     0b10100000,
     0b10100000,
     0b01000000,
     0b00000000,
    }, 
    {// P
     0b11000000,
     0b10100000,
     0b10100000,
     0b11000000,
     0b10000000,
     0b00000000,
    }, 
    {// Q
     0b01000000,
     0b10100000,
     0b10100000,
     0b11000000,
     0b01100000,
     0b00000000,
    }, 
    {// R
     0b11000000,
     0b10100000,
     0b11000000,
     0b10100000,
     0b10100000,
     0b00000000,
    }, 
    {// S
     0b01100000,
     0b10000000,
     0b01000000,
     0b00100000,
     0b11000000,
     0b00000000,
    }, 
    {// T
     0b11100000,
     0b01000000,
     0b01000000,
     0b01000000,
     0b01000000,
     0b00000000,
    }, 
    {// U
     0b10100000,
     0b10100000,
     0b10100000,
     0b10100000,
     0b01100000,
     0b00000000,
    }, 
    {// V
     0b10100000,
     0b10100000,
     0b10100000,
     0b01000000,
     0b01000000,
     0b00000000,
    }, 
    {// W
     0b10100000,
     0b10100000,
     0b11100000,
     0b11100000,
     0b10100000,
     0b00000000,
    }, 
    {// X
     0b10100000,
     0b10100000,
     0b01000000,
     0b10100000,
     0b10100000,
     0b00000000,
    }, 
    {// Y
     0b10100000,
     0b10100000,
     0b01000000,
     0b01000000,
     0b01000000,
     0b00000000,
    }, 
    {// Z
     0b11100000,
     0b00100000,
     0b01000000,
     0b10000000,
     0b11100000,
     0b00000000,
    }, 
    {// [
     0b11000000,
     0b10000000,
     0b10000000,
     0b10000000,
     0b11000000,
     0b00000000,
    }, 
    {// \ // 
     0b10000000,
     0b00000000,
     0b00100000,
     0b00000000,
     0b00001000,
     0b00000000,
    }, 
    {// ]
     0b01100000,
     0b00100000,
     0b00100000,
     0b00100000,
     0b01100000,
     0b00000000,
    }, 
    {// ^
     0b01000000,
     0b10100000,
     0b00000000,
     0b00000000,
     0b00000000,
     0b00000000,
    }, 
    {// _
     0b00000000,
     0b00000000,
     0b00000000,
     0b00000000,
     0b11100000,
     0b00000000,
    }, 
    {// `
     0b10000000,
     0b01000000,
     0b00000000,
     0b00000000,
     0b00000000,
     0b00000000,
    }, 
    {// a
     0b00000000,
     0b11000000,
     0b00100000,
     0b11100000,
     0b01100000,
     0b00000000,
    }, 
    {// b
     0b00000000,
     0b10000000,
     0b11000000,
     0b10100000,
     0b11000000,
     0b00000000,
    }, 
    {// c
     0b00000000,
     0b01100000,
     0b10000000,
     0b10000000,
     0b01100000,
     0b00000000,
    }, 
    {// d
     0b00100000,
     0b00100000,
     0b01100000,
     0b10100000,
     0b01100000,
     0b00000000,
    }, 
    {// e
     0b00000000,
     0b11000000,
     0b11100000,
     0b10000000,
     0b01100000,
     0b00000000,
    }, 
    {// f
     0b01100000,
     0b10000000,
     0b11000000,
     0b10000000,
     0b10000000,
     0b00000000,
    }, 
    {// g
     0b00000000,
     0b01100000,
     0b10100000,
     0b01100000,
     0b00100000,
     0b11000000,
    }, 
    {// h
     0b10000000,
     0b10000000,
     0b11000000,
     0b10100000,
     0b10100000,
     0b00000000,
    }, 
    {// i
     0b01000000,
     0b00000000,
     0b01000000,
     0b01000000,
     0b01000000,
     0b00000000,
    }, 
    {// j
     0b01000000,
     0b00000000,
     0b01000000,
     0b01000000,
     0b01000000,
     0b10000000,
    }, 
    {// k
     0b10000000,
     0b10000000,
     0b10100000,
     0b11000000,
     0b10100000,
     0b00000000,
    }, 
    {// l
     0b01000000,
     0b01000000,
     0b01000000,
     0b01000000,
     0b00100000,
     0b00000000,
    }, 
    {// m
     0b00000000,
     0b10000000,
     0b11100000,
     0b11100000,
     0b11100000,
     0b00000000,
    }, 
    {// n
     0b00000000,
     0b11000000,
     0b10100000,
     0b10100000,
     0b10100000,
     0b00000000,
    }, 
    {// o
     0b00000000,
     0b01000000,
     0b10100000,
     0b10100000,
     0b01000000,
     0b00000000,
    }, 
    {// p
     0b00000000,
     0b11000000,
     0b10100000,
     0b10100000,
     0b11000000,
     0b10000000,
    }, 
    {// q
     0b00000000,
     0b01100000,
     0b10100000,
     0b10100000,
     0b01100000,
     0b00100000,
    }, 
    {// r
     0b00000000,
     0b11000000,
     0b10100000,
     0b10000000,
     0b10000000,
     0b00000000,
    }, 
    {// s
     0b00000000,
     0b01100000,
     0b10000000,
     0b00100000,
     0b11000000,
     0b00000000,
    }, 
    {// t
     0b01000000,
     0b11100000,
     0b01000000,
     0b01000000,
     0b00100000,
     0b00000000,
    }, 
    {// u
     0b00000000,
     0b10100000,
     0b10100000,
     0b10100000,
     0b01100000,
     0b00000000,
    }, 
    {// v
     0b00000000,
     0b10100000,
     0b10100000,
     0b10100000,
     0b01000000,
     0b00000000,
    }, 
    {// w
     0b00000000,
     0b10100000,
     0b10100000,
     0b11100000,
     0b01000000,
     0b00000000,
    }, 
    {// x
     0b00000000,
     0b10010000,
     0b01000000,
     0b01000000,
     0b10010000,
     0b00000000,
    }, 
    {// y
     0b00000000,
     0b10100000,
     0b10100000,
     0b10100000,
     0b01100000,
     0b11000000,
    }, 
    {// z
     0b00000000,
     0b11100000,
     0b01000000,
     0b10000000,
     0b11100000,
     0b00000000,
    }, 
    {// {
     0b01100000,
     0b01000000,
     0b10000000,
     0b01000000,
     0b01100000,
     0b00000000,
    }, 
    {// |
     0b01000000,
     0b01000000,
     0b01000000,
     0b01000000,
     0b01000000,
     0b01000000,
    }, 
    {// }
     0b11000000,
     0b01000000,
     0b00100000,
     0b01000000,
     0b11000000,
     0b00000000,
    }, 
    {// ~
     0b00000000,
     0b01100000,
     0b11000000,
     0b00000000,
     0b00000000,
     0b00000000,
    }, 
    // 
};
