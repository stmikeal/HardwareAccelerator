`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date: 16.12.2025 03:06:02
// Design Name: 
// Module Name: distance
// Project Name: 
// Target Devices: 
// Tool Versions: 
// Description: 
// 
// Dependencies: 
// 
// Revision:
// Revision 0.01 - File Created
// Additional Comments:
// 
//////////////////////////////////////////////////////////////////////////////////


module distance(
    input [0:0] clk_i,
	input [0:0] rst_i,
	input [15:0] x1,
    input [15:0] x2,
    input [15:0] y1,
    input [15:0] y2,
    output [31:0] res
    );
    wire [15:0] maxx;
    wire [15:0] maxy;
    wire [15:0] minx;
    wire [15:0] miny;
    assign maxx = (x2 > x1) ? x2 : x1;
    assign minx = (x2 > x1) ? x1 : x2;
    assign maxy = (y2 > y1) ? y2 : y1;
    assign miny = (y2 > y1) ? y1 : y2;
    wire [15:0] dx;
    wire [15:0] dy;
    assign dx = maxx - minx;
    assign dy = maxy - miny;
    wire [15:0] dmax;
    wire [15:0] dmin;
    assign dmax = (dy > dx) ? dy : dx;
    assign dmin = (dy > dx) ? dx : dy;
    assign res = (251 * dmax + 102 * dmin) >> 8;
    /*
    always @(posedge clk_i) begin 
     $display("%d %d %d %d %d\n", dx, dy, dmax, dmin, res);
     end
     */
endmodule
