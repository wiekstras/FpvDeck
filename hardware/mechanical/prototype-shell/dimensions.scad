/* FpvDeck prototype enclosure parameters, millimetres.
 * CONFIRMED host dimensions: Waveshare mechanical drawing 2026-04-08.
 * Connector clearances are adjustable and must be checked on a 1:1 print. */

host_glass_x = 126.9;        // CONFIRMED
host_glass_y = 70.7;         // CONFIRMED
host_active_x = 110.32;      // CONFIRMED, ±0.15
host_active_y = 62.28;       // CONFIRMED, ±0.15
host_stack_z = 11.6;         // CONFIRMED maximum drawing envelope
host_pcb_x = 118.509;        // CONFIRMED drawing value
host_pcb_y = 64.509;         // CONFIRMED drawing value
host_mount_x = 112.003;      // CONFIRMED drawing value
host_mount_y = 57.0;         // CONFIRMED drawing value
host_thread_nominal = 2.5;   // CONFIRMED: four M2.5 threaded holes

display_offset_x = -8.5;     // DESIGN: leaves a right-hand button rail
wall = 2.2;
fit = 0.5;
corner_r = 7.0;
case_x = 151.0;
case_y = 78.5;
rear_depth = 25.0;
front_depth = 3.4;
lip_depth = 1.6;
lip_thickness = 1.0;

sma_left_y = 6.0;            // DESIGN: RX A exits left enclosure wall
sma_top_x = 30.0;            // DESIGN: RX B exits top-right enclosure wall
sma_barrel_d = 6.5;          // CONFIRMED: Amphenol 132289-12 drawing
sma_cutout_d = 7.1;          // DESIGN: +0.6 diametral FDM clearance
sma_height = 15.0;

jst_body_x = 19.9;           // CONFIRMED: JST S7B-XH-A-1 drawing
jst_body_z = 7.6;            // CONFIRMED side-entry envelope
jst_cutout_x = 20.7;         // DESIGN: +0.8 clearance
jst_cutout_z = 8.4;
jst_x = 22.0;                // DESIGN, provisional HAT placement

button_x = 65.0;
button_y = [-19, 0, 19];
button_d = 7.5;               // DESIGN; actuator cap not frozen
