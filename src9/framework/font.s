.text
.global font_start

@ stored as 256-color 64 bytes/tile with first 8 bytes representing first horizontal line, next 8 bytes next line, ...
@ are kinda in ascii format: first 16 tiles are copies of 0-9 and ABCDEF for easy hexadecimal printing
@ next 16 tiles are some special symbols
@ then normal ASCII characters. font could be compressed but is not for simplicity
.align 2
font_start: .incbin "src9/framework/font.bin"
