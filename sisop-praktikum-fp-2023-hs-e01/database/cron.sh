#!/bin/bash

cd /home/ikikk/sisop-praktikum-fp-2023-hs-e01/database

/usr/bin/zip -rm /home/ikikk/sisop-praktikum-fp-2023-hs-e01/dump/$(date "+%Y-%m-%d")\ $(date +"%H:%M:%S") log.txt /home/ikikk/sisop-praktikum-fp-2023-hs-e01/dump/*.backup

#crontab -e
# 0 * * * * /bin/bash /home/ikikk/sisop-praktikum-fp-2023-hs-e01/database/cron.sh