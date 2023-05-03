# prevent stupidity in deletes:
#I'm actually allowing rm if it has no args -- some install scripts require rm.
#However, I'm printing a reminder to myself to not do that every time I forget.
alias rm='function remove(){ rm "$1"; echo -e "\e[1m\e[91mPlease use trash-put to delete.\n\e[0m"; };remove'
#simpler trashing
alias trash="trash-put"
#prevent unintentional overwrites with cp:
alias cp="cp -i"
#prevent unintentional overwrites with mv:
alias mv="mv -i"
#simple util to remove unwanted whitespace:
alias d2u="dos2unix"
#show open ports:
alias ports="netstat -tulanp"
#show current connections:
alias connections="sudo lsof -nPi"
#better diff
alias diff="colordiff"
#find files taking up too much space (shoot the fat ducks)
alias ducks="du -ch | sort -hr | less"
#alias: ps with lots of info
alias psverbose='ps -Helf'
#psverbose with only my processes
alias psme="psverbose | grep $USER"
#ls by date:
alias lsdate="ls -FlAtr"
#ls by size:
alias lssize="ls -FlASr"
#find that I can remember
alias findeasy="find . -iname"
#quick creation of tags file for ArudPilot
alias tagardu="ctags -R --langmap=c++:.pde -f ardupilot_tags"
#quick mount of ARSENL share
alias mARSENL="sudo mount -t cifs //comfort.ern.nps.edu/ARSENL$ /mnt/ARSENL -o username=maday,domain=ERN,file_mode=0774,dir_mode=0775"
#docker gui
alias dgui="docker run -ti --rm -e DISPLAY=\$DISPLAY -v /tmp/.X11-unix:/tmp/.X11-unix"
alias tpt='touchpad-toggle'
alias tpi="touchpad-indicator &"
#restore desktop session
alias resses="lwsm restore"
#save desktop session
alias saveses="lwsm save"
#bleachbit a linux system optimizer
alias bleach="bleachbit"
#disk space (human readable)
alias spch="df -h"
#print previous return code:
alias prc="echo \$?"

#for some reason this is aliases to git restore --source
#alias grss"git reset --soft"
#git aliases ---------------
alias ga="git add"
alias grs="git reset"
alias gb="git branch"
alias gbd="git branch -d"
alias gbD="git branch -D"
alias gc="git commit"
alias gcm="git commit -m"
alias gcam="git commit --amend -m"
alias gco="git checkout"
alias gcob="git checkout -b"
alias gcp="git cherry-pick"
alias gcpa="git cherry-pick --abort"
alias gcpc="git cherry-pick --continue"
alias gdiff="git diff"
alias gf="git fetch"
alias gg="git grep"
alias gl="git log"
alias grb="git rebase"
alias grba="git rebase --abort"
alias grbc="git rebase --continue"
alias grbi="git rebase --interactive"
alias gsm="git submodule"
alias gsms="git submodule sync"
alias gsmsr="git submodule sync --recursive"
alias gsmu="git submodule update"
alias gsmur="git submodule update --recursive"
alias gsmuir="git submodule update --init --recursive"
alias gs="git status"
alias gw="git whatchanged"
alias grl="git reflog"
alias gls="git ls-files"
alias grv="git remote -v"

#waf aliases
alias wafcbs="./waf configure --board sitl"
alias wafcbco="./waf configure --board CubeOrange"
alias wafbp="./waf plane"
alias wafbc="./waf copter" 

function touchpad-toggle {
    # toggle synaptic touchpad on/off

    # get current state
    SYNSTATE=$(synclient -l | grep TouchpadOff | awk '{ print $3 }')
    #echo $SYNSTATE

    # change to other state
    if [ $SYNSTATE = 0 ]; then
	synclient touchpadoff=1
	echo "touchpad OFF"
    elif [ $SYNSTATE = 1 ]; then
	synclient touchpadoff=0
	echo "touchpad ON"
    else
	echo "Couldn't get touchpad status from synclient"
	exit 1
    fi
}