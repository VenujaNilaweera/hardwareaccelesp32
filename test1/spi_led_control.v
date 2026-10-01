module spi_led_control (
    input  wire clk,       // 50 MHz
    input  wire spi_sclk,
    input  wire spi_mosi,
    input  wire spi_cs,

    output reg  led,
    output reg led2
);

    reg [7:0] spi_data;
    reg [2:0] bit_count;

    // Detect SPI clock edges
    reg sclk_prev;

    always @(posedge clk) begin

        sclk_prev <= spi_sclk;

        // CS active
        if (!spi_cs) begin

            // Detect rising edge of SPI clock
            if (spi_sclk && !sclk_prev) begin

                spi_data <= {spi_data[6:0], spi_mosi};

                if (bit_count == 3'd7) begin
                    bit_count <= 3'd0;

                    // Commands
                    case ({spi_data[6:0], spi_mosi})

                        8'h01: begin
                            led  <= 1'b1;
                            led2 <= 1'b0;
                        end

                        8'h00: begin
                            led  <= 1'b0;
                            led2 <= 1'b0;
                        end

                        8'h02: begin
                            led  <= 1'b0;
                            led2 <= 1'b1;
                        end

                        8'h03: begin
                            led  <= 1'b1;
                            led2 <= 1'b1;
                        end

                    endcase

                end
                else begin
                    bit_count <= bit_count + 1'b1;
                end

            end

        end
        else begin
            bit_count <= 3'd0;
        end

    end

endmodule