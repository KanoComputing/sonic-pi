#!/bin/bash
#
#  Starts make-music with a progress dialog.
#  If a make-music instance is already running, give it the focus and exit.
#  Progress dialog requests focus all the time so it doesn't get obscured by overlapping windows.
#

APPNAME="Make Music - default"
DIALOG_TITLE="Make Music"
PIDFILE=/tmp/start-make-music.pid
CONTENT_DIR=/home/$USER/Music-content

# Workspaces to be created first time Make-Music is launched
function init() 
{
    WORKSPACE_DIR=/home/$USER/.make-music/workspaces/default

    echo "Creating workspace directories"
    mkdir -pv $WORKSPACE_DIR/one
    mkdir -pv $WORKSPACE_DIR/two
    mkdir -pv $WORKSPACE_DIR/three
    mkdir -pv $WORKSPACE_DIR/four
    mkdir -pv $WORKSPACE_DIR/five
    mkdir -pv $WORKSPACE_DIR/six
    mkdir -pv $WORKSPACE_DIR/seven
    mkdir -pv $WORKSPACE_DIR/eight

    echo "Creating workspaces"
    touch $WORKSPACE_DIR/one/1.spi
    touch $WORKSPACE_DIR/two/1.spi
    touch $WORKSPACE_DIR/three/1.spi
    touch $WORKSPACE_DIR/four/1.spi
    touch $WORKSPACE_DIR/five/1.spi
    touch $WORKSPACE_DIR/six/1.spi
    touch $WORKSPACE_DIR/seven/1.spi
    touch $WORKSPACE_DIR/eight/1.spi

    echo "Creating content directory"
    mkdir -pv $CONTENT_DIR

    echo "Giving read-access"
    sudo chmod +r /dev/input/event*
}

# remove temporary pid file always at exit time
function cleanup {

    if [ -f $PIDFILE ]; then
        rm $PIDFILE
    fi
}
trap cleanup EXIT

# First time Make-Music is launched, call init() 
if [ ! -d $CONTENT_DIR ]; then
    init
fi

# Avoid multiple make-music instances
if [ -f $PIDFILE ]; then
    echo "Start-Make music is starting."
    exit 0
else
    touch $PIDFILE
fi

wdata=`xwininfo -root -tree | grep "$APPNAME"`
rc=$?
if [ $rc -eq 0 ]; then
    echo "$APPNAME is already running, raising focus."
    wid=`echo $wdata | awk '{print $1}'`
    xdotool windowraise $wid
    exit 1
fi

/opt/make-music/app/gui/application &
MUSICPID=$!

(
echo "#Make Music is loading..."

for I in 10 15 20 25 30 35 40 45 50 55 60 65 70 75 80 85 90 95 99; do
    sleep 1
    xwininfo -root -tree | grep "$APPNAME"
    rc=$?

    dialogid=`xwininfo -name "$DIALOG_TITLE" |grep "Window id:" | awk '{print $4}'`
    if [ $? -eq 0 ] ; then
	xdotool windowraise $dialogid
    fi

    if [ $rc -eq 0 ]; then

	# maximize is not something xdotool can do - it's a window manager feature
	# hence we are resing it full screen, avoiding the gap on the task bar.
	wid=`xdotool search "$APPNAME"`
	xdotool windowmove $wid 0 0
	xdotool windowsize $wid 1024 727

	echo "99"
	echo "#Ready"
	sleep 3
	break
    fi
    echo "$I"
done

if [ $rc -ne 0 ]; then
    echo "#$DIALOG_TITLE could not be started - RC=$rc"
    sleep 3
fi

echo "100"
) |
zenity --progress --no-cancel --title="$DIALOG_TITLE" --text="" --percentage=0 --width=400 --height=125 --auto-close --timeout=30 --auto-kill

if [ "$?" = -1 ] ; then
    zenity --error
    --text="Start failed."
fi
echo "$APPNAME PID=$MUSICPID"
exit $MUSICPID
