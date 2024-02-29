# Ani Gui Pi
The goal of this program is to be able to watch shows on a raspberry pi zero. Currently the plan is to ssh into the raspberry pi zero and pass a video url to cvlc. This program will be able to search shows (similar to ani-cli) with either ncurses as the tui and either gtk or a python library (I'm still deciding) as the gui. After getting a show this program will connect the the raspberry pi over ssh (using a sshkey) and will be able to control the video (seeking, pausing, volume, closing, etc).
