#!/bin/sh

### BEGIN INIT INFO
# Provides:          locate_server
# Required-Start:    $network $local_fs
# Required-Stop:
# Default-Start:     2 3 4 5
# Default-Stop:      0 1 6
# Short-Description: locate server
# Description:       locate server
### END INIT INFO

/home/toor/LocServer/locate_server &
