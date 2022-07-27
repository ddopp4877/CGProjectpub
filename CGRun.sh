#!/bin/bash

echo "starting CG run" | sendmail dpd4k4@umsystem.edu
python3 initProject.py


python3 CGProject.py &>out.txt && echo "CG done in $(($SECONDS/60)) minutes and $(($SECONDS - $SECONDS/60 *60)) seconds" | sendmail dpd4k4@umsystem.edu&
