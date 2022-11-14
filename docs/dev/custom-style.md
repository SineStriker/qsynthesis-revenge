# Customized Style Instruction

## Overview

QSynthesis uses Qt Stylesheet mainly to change appearance, and some extra syntax is added to improve style writing and updating efficiency.

This document only briefly describes how to use the additional syntax, and you can refer to the implementation code for the syntax in `Register` folder.

## Syntax

### Escapes

|Original               |Escaped                                      |
|----                   |----                                         |
|fix-xxx: \<content\>;  |min-xxx: \<content\>; max-xxx: \<content\>;  |
|...                    | ...                                         |

### QMargins

+ Usage: Qt class to define margins but not supported in QMetaType.
+ Format: **qmargins(*left*, *top*, *right*, *bottom*)**
+ Examples:
  + qmargins(0, 0, 0, 0)
  + qmargins(10px, 10px, 10px, 10px)
  + 0
  + 5px
+ Details:
  + The *px* unit can be omitted even if the value is not 0.

### QSvgUri

+ Abstract: A substitute for the CSS function *url* to import an SVG file and replace the *currentColor* string in it. An instance of QSvgUri can create a svg QIcon of the specified color by CSvgIconEngine, which is a subclass of QIconEngine.
+ Format: **svg(*path*, *color_str*)**
+ Examples:
  + svg(:/images/a.svg, #000000)
  + svg("C:/b.svg", black)
  + svg("D:/c.svg")
+ Details:
  + The path argument can be quoted or unquoted.
  + The color argument is a string and is replaced directly into the file without any modification.
  + If the color argument is not specified or the SVG file does not contain the currentColor string, no color will be replaced.

### QTypeFace

+ Abstract: A struct containing a font and a color.
+ Format: **qtypeface(*color*, *size*, *weight*...)**
+ Examples:
  + qtypeface(#000000, 10px, 75)
  + qtypeface(rgb(0, 0, 0), 12.5)
+ Details:
  + The color parameter is parsed into QColor by CSS syntax format.
  + The size parameter is parsed to pixel size if it has *px* unit, otherwise to point size.
  + All arguments starting with the third argument are consolidated into a string, which will be parsed to QFont using **QFont::fromString**.


### QDataUri

+ Abstract: Implement the functionality of *url* in Qt stylesheet.
+ Format: **url(data:*format*;charset=*charset_name*, *content*)**
+ Examples:
  + url(data:image/svg+xml;charset=utf-8, \<svg code\>)
  + url(data:image/png;charset=base64, \<png code\>)