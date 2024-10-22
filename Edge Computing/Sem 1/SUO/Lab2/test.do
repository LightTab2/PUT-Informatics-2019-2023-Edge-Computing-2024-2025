vlib work
quit -sim
catch { vcom -2008 -autoorder *.vhd }
vsim -voptargs=+acc $1
view wave 
view signals
add wave *

run $2

wave zoom full
