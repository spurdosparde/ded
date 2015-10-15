# ded
Digital Encrypted Diary

A feather light text mode diary program in C for linux, that encrypts every entry (AES-256) before storing it in a sqlite database.
It uses vim as the editor and ncurses.

You'll be prompted to enter a key at the start, it will be used during the session to encrypt/decrypt the entries.
Obviously, wrong key = garbage on your screen.

Dependencies:

            - openssl
            - ncurses
            - sqlite3
            - libconfig
            
How to build:

            gcc -o ded *.c -lcrypto -lsqlite3 -lncursesw -lconfig

It's licensed under the WTFPL license:

            DO WHAT THE FUCK YOU WANT TO PUBLIC LICENSE
                        Version 2, December 2004:
            Everyone is permitted to copy and distribute verbatim or modified
            copies of this license document, and changing it is allowed as long
            as the name is changed.

            DO WHAT THE FUCK YOU WANT TO PUBLIC LICENSE
            TERMS AND CONDITIONS FOR COPYING, DISTRIBUTION AND MODIFICATION

            0. You just DO WHAT THE FUCK YOU WANT TO.

            THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
            IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
            FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
            AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
            LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
            OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
            THE SOFTWARE.
