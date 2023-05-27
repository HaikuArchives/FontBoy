Fontboy
=======

© 1999-2003 Oliver 'Madison' Kohl

Introduction
============
Fontboy is a small application to show your installed fonts in Haiku. You can get additional information of the selected font and a preview of the complete Unicode charset. Fontboy allows you to adjust the appearance to your desired needs, and it's fast because Haiku is.

![FontBoy screenshot](FontBoy.png "FontBoy")

Features
========
- Watches all your installed fonts
- Browse the complete Unicode character set of a font
- Magnified view of single characters
- Drag & drop support
- Tooltips
- Highly customizeable

How to use Fontboy
==================
In the main window, all installed fonts will be displayed with some sample text, their name, and the style. Single-click to select a font. Double-click to get more information about a font. A right mouse click will show a popup menu where you can also open the details window or refresh the font list after installing new fonts.

The details window shows you further information about the selected font and a graphical view of its Unicode character set. The selected character in the matrix will be shown in a bigger view on the left side. Use the mouse or cursor keys to change the selection. Use upper right navigation buttons or PageUp/PageDown keys to change page. Try dragging & dropping characters!

Preferences
===========
To change the application settings, open the preferences from the menu Edit.
- General let you toggle the splashscreen when starting Fontboy and turn off the ToolTips.  
- List let you adjust the appearance of the fontlist in the main window.  
- Details let you adjust the appearance of the detail window.  
- When Live update! is selected all settings will be applied immediately.

Tips
====
- Drag & drop of a font in main window also sends a FFont object, but there are not a lot of applications that support it.

Contact
=======
Web: [fontboy.sourceforge.net](http://fontboy.sourceforge.net)  
EMail: fontboyapp@gmail.com  
Twitter: [@fontboy](https://twitter.com/fontboy)

Credits
=======
Splashscreen image by Thomas 'tHOM!' Einweg  
Coding & additional graphics by Oliver 'Madison' Kohl

Thanks and regards must go to:
- Dianne Hackborn (and others) for the FFont
- Marco Nelissen for his BubbleHelper class
- Eric Shepherd for his TPreferences class
- YNOP Talton for SplitPane class
