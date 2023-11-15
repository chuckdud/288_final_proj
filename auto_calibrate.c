/**
 *
 * rough idea of a way to improve the calibration of ir sensor
 *
 * still pseudo-code
 *
 */

int main() {
	servo_move(180);
	float ping1 = ping_read();
	while (abs(ping1 - 10) > 1) {
		move_forward(100);
		ping1 = ping_read();	
	}
	ping1 = ping_read();
	int IR_1 = adc_read();

	float ping2 = ping_read();
	while (abs(ping2 - 45) > 1) {
		move_backward(100);
		ping2 = ping_read();
	}
	ping2 = ping_read();
	int IR_2 = adc_read();

	IR_R = IR_FindR(ping1, ping2, IR_1, IR_2);
	IR_C = IR_FindC(IR_R, ping1, IR_1);

	// then wait for some uart comms to tell us to proceed
}
