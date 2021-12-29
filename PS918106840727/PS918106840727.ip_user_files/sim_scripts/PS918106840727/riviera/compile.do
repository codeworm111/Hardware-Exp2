vlib work
vlib riviera

vlib riviera/xilinx_vip
vlib riviera/axi_infrastructure_v1_1_0
vlib riviera/axi_vip_v1_1_5
vlib riviera/processing_system7_vip_v1_0_7
vlib riviera/xil_defaultlib

vmap xilinx_vip riviera/xilinx_vip
vmap axi_infrastructure_v1_1_0 riviera/axi_infrastructure_v1_1_0
vmap axi_vip_v1_1_5 riviera/axi_vip_v1_1_5
vmap processing_system7_vip_v1_0_7 riviera/processing_system7_vip_v1_0_7
vmap xil_defaultlib riviera/xil_defaultlib

vlog -work xilinx_vip  -sv2k12 "+incdir+E:/Xilinx2019/Vivado/2019.1/data/xilinx_vip/include" \
"E:/Xilinx2019/Vivado/2019.1/data/xilinx_vip/hdl/axi4stream_vip_axi4streampc.sv" \
"E:/Xilinx2019/Vivado/2019.1/data/xilinx_vip/hdl/axi_vip_axi4pc.sv" \
"E:/Xilinx2019/Vivado/2019.1/data/xilinx_vip/hdl/xil_common_vip_pkg.sv" \
"E:/Xilinx2019/Vivado/2019.1/data/xilinx_vip/hdl/axi4stream_vip_pkg.sv" \
"E:/Xilinx2019/Vivado/2019.1/data/xilinx_vip/hdl/axi_vip_pkg.sv" \
"E:/Xilinx2019/Vivado/2019.1/data/xilinx_vip/hdl/axi4stream_vip_if.sv" \
"E:/Xilinx2019/Vivado/2019.1/data/xilinx_vip/hdl/axi_vip_if.sv" \
"E:/Xilinx2019/Vivado/2019.1/data/xilinx_vip/hdl/clk_vip_if.sv" \
"E:/Xilinx2019/Vivado/2019.1/data/xilinx_vip/hdl/rst_vip_if.sv" \

vlog -work axi_infrastructure_v1_1_0  -v2k5 "+incdir+../../../../PS918106840727.srcs/sources_1/bd/PS918106840727/ipshared/ec67/hdl" "+incdir+../../../../PS918106840727.srcs/sources_1/bd/PS918106840727/ipshared/8c62/hdl" "+incdir+../../../../PS918106840727.srcs/sources_1/bd/PS918106840727/ip/PS918106840727_processing_system7_0_1" "+incdir+E:/Xilinx2019/Vivado/2019.1/data/xilinx_vip/include" \
"../../../../PS918106840727.srcs/sources_1/bd/PS918106840727/ipshared/ec67/hdl/axi_infrastructure_v1_1_vl_rfs.v" \

vlog -work axi_vip_v1_1_5  -sv2k12 "+incdir+../../../../PS918106840727.srcs/sources_1/bd/PS918106840727/ipshared/ec67/hdl" "+incdir+../../../../PS918106840727.srcs/sources_1/bd/PS918106840727/ipshared/8c62/hdl" "+incdir+../../../../PS918106840727.srcs/sources_1/bd/PS918106840727/ip/PS918106840727_processing_system7_0_1" "+incdir+E:/Xilinx2019/Vivado/2019.1/data/xilinx_vip/include" \
"../../../../PS918106840727.srcs/sources_1/bd/PS918106840727/ipshared/d4a8/hdl/axi_vip_v1_1_vl_rfs.sv" \

vlog -work processing_system7_vip_v1_0_7  -sv2k12 "+incdir+../../../../PS918106840727.srcs/sources_1/bd/PS918106840727/ipshared/ec67/hdl" "+incdir+../../../../PS918106840727.srcs/sources_1/bd/PS918106840727/ipshared/8c62/hdl" "+incdir+../../../../PS918106840727.srcs/sources_1/bd/PS918106840727/ip/PS918106840727_processing_system7_0_1" "+incdir+E:/Xilinx2019/Vivado/2019.1/data/xilinx_vip/include" \
"../../../../PS918106840727.srcs/sources_1/bd/PS918106840727/ipshared/8c62/hdl/processing_system7_vip_v1_0_vl_rfs.sv" \

vlog -work xil_defaultlib  -v2k5 "+incdir+../../../../PS918106840727.srcs/sources_1/bd/PS918106840727/ipshared/ec67/hdl" "+incdir+../../../../PS918106840727.srcs/sources_1/bd/PS918106840727/ipshared/8c62/hdl" "+incdir+../../../../PS918106840727.srcs/sources_1/bd/PS918106840727/ip/PS918106840727_processing_system7_0_1" "+incdir+E:/Xilinx2019/Vivado/2019.1/data/xilinx_vip/include" \
"../../../../PS918106840727.srcs/sources_1/bd/PS918106840727/ip/PS918106840727_processing_system7_0_1/sim/PS918106840727_processing_system7_0_1.v" \
"../../../../PS918106840727.srcs/sources_1/bd/PS918106840727/sim/PS918106840727.v" \

vlog -work xil_defaultlib \
"glbl.v"
