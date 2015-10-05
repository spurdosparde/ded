# ded
Digital Encrypted Diary

A feather light text mode diary program for linux, that encrypts every entry (AES-256, openssl implementation) before storing it in a sqlite database.
It uses vim as the editor and ncurses.

You'll be prompted to enter a key at the start, it will be used during the session to encrypt/decrypt the entries.
Obviously, wrong key = garbage on your screen.

Dependencies:
            - openssl
            - ncurses
            - sqlite3
            
How to build:
            DO WHAT THE FUCK YOU WANT TO PUBLIC LICENSE

It's licensed under the WTFPL license:

            DO WHAT THE FUCK YOU WANT TO PUBLIC LICENSE
                    Version 2, December 2004

 Everyone is permitted to copy and distribute verbatim or modified
 copies of this license document, and changing it is allowed as long
 as the name is changed.

            DO WHAT THE FUCK YOU WANT TO PUBLIC LICENSE
   TERMS AND CONDITIONS FOR COPYING, DISTRIBUTION AND MODIFICATION

  0. You just DO WHAT THE FUCK YOU WANT TO.
