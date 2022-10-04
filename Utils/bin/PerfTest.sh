#!/bin/bash
#ulimit -c unlimited 待完善
WORK_PATH=$(cd $(dirname $0); pwd)
var=/log
LOG_PATH=${WORK_PATH}${var}
echo $LOG_PATH
mkdir -p $LOG_PATH
nohup $WORK_PATH/consumer c1 > $LOG_PATH/consumer1.log 2>&1 &
nohup $WORK_PATH/producer p1 > $LOG_PATH/producer1.log 2>&1 &