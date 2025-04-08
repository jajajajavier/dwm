#!/usr/bin/env bash

col_bg="#282c34"
col_fg="#abb2bf"
col_black="#5c6370"
col_blue="#61afef"
col_cyan="#56b6c2"
col_green="#98c379"
col_magenta="#c678dd"
col_red="#e06c75"
col_yellow="#d19a66"
col_white="#ffffff"
col_gray="#353535"

print_date () {
  printf "^b$col_blue^^c$col_bg^ 󰃭 "
  printf "^b$col_gray^^c$col_blue^ $(date '+%a %d %b %Y')"
}

clock () {
  printf "^b$col_magenta^^c$col_bg^  "
  printf "^b$col_gray^^c$col_magenta^$(date '+%H:%M:%S')"
}

battery () {
  perc=`upower -i $(upower -e | grep BAT) | grep "percentage:" | sed 's/[^0-9]*//g'`
  state=`upower -i $(upower -e | grep BAT) | grep "state:" | sed 's/ *state: *//g'`

  printf "^b$col_bg^^c$col_green^"
  if [ $state == "charging" -o $state == "fully-charged" ]; then
    printf " "
  else
    if [ $perc -le 5 ]; then printf "^c$col_red^  "
    elif [ $perc -le 25 ]; then printf "^c$col_red^  "
    elif [ $perc -le 50 ]; then printf "  "
    elif [ $perc -le 75 ]; then printf "  "
    else printf "  "
    fi
  fi

  printf "$perc%%"
}

light () {
  perc=`brightnessctl i | grep -o -E "[0-9]+%" | sed 's/ *% *//g'`
  printf "^b$col_bg^^c$col_white^"

  if [ $perc -le 25 ]; then printf "󰃞 "
  elif [ $perc -le 50 ]; then printf "󰃝 "
  elif [ $perc -le 75 ]; then printf "󰃟 "
  else printf "󰃠 "
  fi
  
  printf "$perc%%"
}

audio () {
  perc=`pamixer --get-volume`
  muted=`pamixer --get-mute`
  
  printf "^b$col_bg^^c$col_cyan^"
  if [ $muted == "true" ]; then
    printf " "
  else
    if [ $perc -le 32 ]; then printf " "
    elif [ $perc -le 64 ]; then printf " "
    else printf " "
    fi

    printf " $perc%%"
  fi
}

music () {
  aux=`playerctl metadata &> /dev/null`
  if [ $? -ne 0 ]; then
	  return
  fi

  title=`playerctl metadata xesam:title`
  artist=`playerctl metadata xesam:artist`
  album=`playerctl metadata xesam:album`
  out=" $title | $artist"

  if [[ $album != "" ]]; then
    out+=", $album"
  fi

  printf "^b$col_magenta^^c$col_bg^"
  if [  `playerctl status` == "Playing" ]; then
	  printf " 󰎇 "
  else
	  printf " 󰏤 "
  fi

  max_len=64
  if [[ ${#out} -gt $max_len ]]; then
    out="${out:0:$max_len}..."
  fi

  printf "^b$col_gray^^c$col_fg^"
  printf "%s" "$out"

  if [ `playerctl shuffle` == "On" ]; then
	  printf "^b$col_magenta^^c$col_bg^ 󰒟 "
  else
    printf "^b$col_bg^   "
  fi


  printf "^b$col_bg^"
}

mem() {
  printf "^c$col_bg^^b$col_yellow^ MEM "
  printf "^c$col_yellow^^b$col_gray^ $(free -h | awk '/^Mem/ { print $3 }' | sed s/i//g)"
}

cpu() {
  cpu_val=$(grep -o "^[^ ]*" /proc/loadavg)
  printf "^c$col_bg^ ^b$col_cyan^ CPU"
  printf "^c$col_cyan^ ^b$col_gray^ $cpu_val"
}

while true; do
  sleep 1 && xsetroot -name "$(music)     $(audio)   $(battery)   $(light)   $(mem)$(cpu) $(print_date) $(clock) " &> /dev/null

  if [ $? != 0 ]; then
    break
  fi
done 
