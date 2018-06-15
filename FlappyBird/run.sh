#!/bin/bash  
function random(){  
    min=$1  
    max=`expr $2 - $1`  
    num=$RANDOM  
    temp=$(( $num % $max ))  
    retnum=$(( $temp + $min ))  
    echo $retnum  
}  
function print_bird()  
{  
    x=$1  
    y=$2  
    tput setf 6  
    tput bold  
    tput sc  
    tput cup $y $x  
    printf '   ______';((y++));tput cup $y $x  
    printf ' _/   /  \ ';((y++));tput cup $y $x  
    printf '/  \  \__0|_';((y++));tput cup $y $x  
    printf '\__|    /___\ ';((y++));tput cup $y $x  
    printf ' \______\___/';  
    tput rc  
}  
# x r h is_top  
function print_pillar(){  
    if [ $3 -gt 0 ]; then  
        left=`expr $1 - $2`  
        right=`expr $1 + $2`  
        width=`expr $right -  $left`  
        top=0  
        bottom=$3  
  
        is_top=$4  
        if [ $is_top -eq 0 ]; then  
            bottom=$( tput lines )  
            top=`expr $bottom - $3`  
        fi  
        tput sc  
  
        temp=`expr $width - 2`  
        command=$( printf '|%%-%ds|' $temp )  
        for i in $( seq $top $bottom );  
        do  
            #plot the bottom pillar's top line  
            if [ $i -eq $top ] && [ $is_top -eq 0 ]; then  
                temp=`expr $top - 1`  
                tput cup $temp $left  
                for j in $( seq $width );  
                do  
                    printf "_"  
                done  
            fi  
            #plot the pillar's stick  
            tput cup $i $left  
            printf "$command" " "  
            #plot the top pillar's bottom line  
            if [ $i -eq $bottom ] && [ $is_top -gt 0 ]; then  
                temp=`expr $left + 1`  
                tput cup $i $temp  
                temp=`expr $width - 2`  
                for j in $( seq $temp );  
                do  
                    printf "_"  
                done  
            fi  
        done  
        tput rc  
    fi  
}  
#*********************************  
# mode set  
##################################  
tput clear  
echo -e "\033[?25l"  
##################################  
# parameters set  
##################################  
init_g=10  
init_x=5  
init_y=`expr $( tput lines ) / 2`  
init_v=0  
  
speed=2  
space=30  
bird_r=5  
line_top=8  
line_bot=30  
left_space_min=2  
left_space_max=10  
pillar_w=4  
bird_w=13  
bird_h=4  
g_bird_x=$init_x   
g_bird_y=$init_y   
g_bird_v=$init_v   
g_bird_a=$init_g  
  
g_line=()  
g_line_x=()  
g_pillars_x=()  
g_pillars_r=()  
g_pillars_h=()  
g_pillars_is_top=()  
##################################  
# init the pillars  
##################################  
  
bottom_border=$( tput lines )  
right_border=$( tput cols )  
temp=`expr $right_border / $space`  
point_num=`expr $temp + 3`  
for init_t in $(seq $point_num);  
do  
    temp=$(($init_t*$space))  
    pass_pos=$(random $line_top $line_bot)  
    g_line=("${g_line[@]}" $pass_pos)  
    g_line_x=("${g_line_x[@]}" $temp)  
    if [ $(($RANDOM%10)) -lt 8 ]; then  
        left_space=$( random $left_space_min $left_space_max )  
        # add the top pillar  
        temp=$(($init_t*$space))  
        g_pillars_x=("${g_pillars_x[@]}" $temp)  
        g_pillars_r=("${g_pillars_r[@]}" $pillar_w)  
        temp=$(($pass_pos-$bird_r -$left_space))  
        if [ $temp -lt 1 ];then  
            temp=0  
        fi  
        g_pillars_h=("${g_pillars_h[@]}" $temp)  
        g_pillars_is_top=("${g_pillars_is_top[@]}" 1)  
        # add the bottom pillar  
        temp=$(($init_t*$space))  
        g_pillars_x=("${g_pillars_x[@]}" $temp)  
        g_pillars_r=("${g_pillars_r[@]}" $pillar_w)  
        temp=$(($bottom_border-$pass_pos-$bird_r-$left_space))  
        if [ $temp -lt 1 ];then  
            temp=0  
        fi  
        g_pillars_h=("${g_pillars_h[@]}" $temp)  
        g_pillars_is_top=("${g_pillars_is_top[@]}" 0)  
    fi  
done  
######################################  
# make the pillars move  
######################################  
loop_num=0  
while :  
do  
    loop_num=`expr $loop_num + 1`  
    # read in the user's operation  
    is_jump=0  
    read -n 1 -t 0.01 $letter && is_jump=1  
    # update the pillars' position  
    for p_id in $( seq ${#g_pillars_x[@]} );  
    do  
        id=`expr $p_id - 1`  
        g_pillars_x[$id]=`expr ${g_pillars_x[$id]} - 1`  
    done  
    for l_id in $( seq ${#g_line_x[@]} );  
    do  
        id=`expr $l_id - 1`  
        g_line_x[$id]=`expr ${g_line_x[$id]} - 1`  
    done      
    # update the bird's position  
    if [ $is_jump -eq 0 ];then  
        g_bird_v=$(( $g_bird_a / 4 + $g_bird_v ))  
    else  
        g_bird_v=-8  
    fi  
    g_bird_y=$(( $g_bird_y + $g_bird_v / 4 ))  
  
    # check whether the game is over  
    pillar_num=${#g_pillars_x[@]}  
    is_over=0  
    if [ $g_bird_y -lt 0 ] || [ $g_bird_y -gt $bottom_border ]; then   
        is_over=1  
    fi  
    for p_id in $( seq $pillar_num ); do  
        {  
        id=`expr $p_id - 1`  
        temp_pillar_left=`expr ${g_pillars_x[$id]} - ${g_pillars_r[$id]}`  
        temp_pillar_right=`expr ${g_pillars_x[$id]} + ${g_pillars_r[$id]}`  
        temp_pillar_pos=`expr $bottom_border - ${g_pillars_h[$id]}`  
        temp_bird_right=`expr $g_bird_x + $bird_w`  
        temp_bird_bot=`expr $g_bird_y + $bird_h`  
        if [ ${g_pillars_is_top[$id]} -gt 0 ]; then  
            if [ $temp_bird_right -gt $temp_pillar_left ] && [ $g_bird_x -lt $temp_pillar_right ] && [ $g_bird_y -lt ${g_pillars_h[$id]} ]; then  
                tput clear  
                is_over=1  
                echo "TOUCH THE TOP PILLAR"  
                exit  
            fi  
        else   
            if [ $temp_bird_right -gt $temp_pillar_left  ] && [ $g_bird_x -lt $temp_pillar_right  ] && [ $temp_bird_bot -gt $temp_pillar_pos ]; then  
                tput clear  
                is_over=1  
                echo "TOUCH THE BOTTOM PILLAR"  
                exit  
            fi  
        fi    
        }   
    done  
    # if bird touch the pillar then the game over  
    if [ $is_over -gt 0 ]; then  
        tput clear  
        echo "GAME OVER"  
        exit  
    fi  
    # check whether need to add one new pillar  
    if [ ${g_line_x[0]} -lt 0 ]; then  
        # delete the line point and remove the pillar if exist  
        g_line_x=(${g_line_x[@]:1:${#g_line_x[@]}})  
        g_line=(${g_line[@]:1:${#g_line[@]}})  
    fi  
    if [ ${g_pillars_x[0]} -lt 0 ]; then  
        g_pillars_x=(${g_pillars_x[@]:1:${#g_pillars_x[@]}})  
        g_pillars_r=(${g_pillars_r[@]:1:${#g_pillars_r[@]}})  
        g_pillars_h=(${g_pillars_h[@]:1:${#g_pillars_h[@]}})  
        g_pillars_is_top=(${g_pillars_is_top[@]:1:${#g_pillars_is_top[@]}})  
    fi  
      
    # add a new line point and a new pillar if fall into possibility  
    if [ ${#g_line_x[@]} -lt $point_num ]; then  
        last_id=`expr ${#g_line_x[@]} - 1`  
        temp=`expr ${g_line_x[$last_id]} + $space`  
        pass_pos=$(random $line_top $line_bot)  
        g_line=("${g_line[@]}" $pass_pos)  
        g_line_x=("${g_line_x[@]}" $temp)  
        if [ $(random 0 10) -lt 18 ]; then   
            left_space=$( random $left_space_min $left_space_max)  
            g_pillars_x=("${g_pillars_x[@]}" $temp)  
            g_pillars_r=("${g_pillars_r[@]}" 3)  
            g_pillars_x=("${g_pillars_x[@]}" $temp)  
            g_pillars_r=("${g_pillars_r[@]}" 3)  
  
            temp=$(($pass_pos - $bird_r - $left_space))  
            g_pillars_h=("${g_pillars_h[@]}" $temp)  
            g_pillars_is_top=("${g_pillars_is_top[@]}" 1)  
            temp=$(($bottom_border - $pass_pos - $bird_r - $left_space))   
            g_pillars_h=("${g_pillars_h[@]}" $temp)  
            g_pillars_is_top=("${g_pillars_is_top[@]}" 0)  
        fi  
    fi  
    # display the pillars  
    tput clear  
    if [ ${#g_pillars_h[@]} -gt 0 ] && [ ${#g_pillars_x[@]} -eq ${#g_pillars_r[@]} ] && [ ${#g_pillars_r[@]} -eq ${#g_pillars_h[@]} ] && [ ${#g_pillars_x[@]} -eq ${#g_pillars_is_top[@]} ]; then  
        for p_id in $( seq ${#g_pillars_x[@]} ); do  
            {  
  
                id=`expr $p_id - 1`  
                cur_x=${g_pillars_x[$id]}   
                cur_r=${g_pillars_r[$id]}   
                cur_h=${g_pillars_h[$id]}   
                cur_is_top=${g_pillars_is_top[$id]}  
                temp_pillar_right=`expr $cur_x + $cur_r`  
                if [ $temp_pillar_right -lt $right_border ] && [ $cur_x -gt $cur_r ]; then  
                    print_pillar $cur_x $cur_r $cur_h $cur_is_top  
                fi  
            }  
        done  
    fi  
    print_bird $g_bird_x $g_bird_y   
done 
