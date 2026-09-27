module  delay500ms (
    input wire clk,
    output reg led
);

    reg [25:0] counter = 26'd0;

    always @(posedge clk) begin
        if (counter == 26'd24_999_999) begin
            counter <= 26'd0;
            led <= ~led;
        end
        else begin
            counter <= counter + 1'b1;
        end
    end

endmodule