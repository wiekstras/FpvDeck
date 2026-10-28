include <dimensions.scad>

$fn = 64;
part = "assembly"; // assembly, product, front, rear, fit_check

module rounded_prism(size, radius) {
    linear_extrude(height=size[2])
        offset(r=radius)
            square([size[0]-2*radius, size[1]-2*radius], center=true);
}

module front_bezel() {
    difference() {
        rounded_prism([case_x, case_y, front_depth], corner_r);
        // Only the confirmed active display area is exposed.
        translate([display_offset_x, 0, -0.1])
            rounded_prism([host_active_x+0.8, host_active_y+0.8,
                           front_depth+0.2], 1.5);
        // UP / OK / DOWN front openings on the dedicated control rail.
        for (y=button_y)
            translate([button_x, y, -0.1])
                cylinder(h=front_depth+0.2, d=button_d);
    }
    // Locating lip. It does not replace screws and is deliberately loose.
    translate([0, 0, front_depth])
        difference() {
            rounded_prism([case_x-2*wall, case_y-2*wall, lip_depth],
                           corner_r-wall);
            rounded_prism([case_x-2*(wall+lip_thickness),
                           case_y-2*(wall+lip_thickness), lip_depth+0.1],
                           corner_r-wall-lip_thickness);
        }
}

module mount_boss(x, y) {
    translate([display_offset_x+x, y, wall])
        difference() {
            cylinder(h=4.2, d=7.0);
            cylinder(h=4.3, d=2.8);
        }
}

module rear_shell() {
    difference() {
        rounded_prism([case_x, case_y, rear_depth], corner_r);
        translate([0, 0, wall])
            rounded_prism([case_x-2*wall, case_y-2*wall,
                           rear_depth-wall+0.1], corner_r-wall);

        // Orthogonal antenna placement: RX A left, RX B top-right.
        translate([-case_x/2-0.2, sma_left_y, sma_height])
            rotate([0,90,0]) cylinder(h=wall+0.6, d=sma_cutout_d);
        translate([sma_top_x, case_y/2+0.2, sma_height])
            rotate([90,0,0]) cylinder(h=wall+0.6, d=sma_cutout_d);

        // Bottom side-entry JST-XH 7-pin balance-lead opening.
        translate([display_offset_x+jst_x, -case_y/2-0.2,
                   rear_depth-jst_cutout_z/2-3.0])
            cube([jst_cutout_x, wall+0.6, jst_cutout_z], center=true);
    }
    for (x=[-host_mount_x/2, host_mount_x/2])
        for (y=[-host_mount_y/2, host_mount_y/2]) mount_boss(x,y);
}

module host_keepout() {
    color([0.1,0.5,0.65,0.55])
        translate([display_offset_x,0,front_depth])
            rounded_prism([host_glass_x,host_glass_y,host_stack_z],3.0);
}

module hat_keepout() {
    color([0.1,0.65,0.35,0.50])
        translate([display_offset_x,0,front_depth+host_stack_z+2.5])
            cube([host_pcb_x,host_pcb_y,1.6],center=true);
}

module connector_keepouts() {
    color([0.85,0.65,0.15,0.8]) {
        translate([-case_x/2,sma_left_y,sma_height])
            rotate([0,90,0]) cylinder(h=15,d=sma_barrel_d,center=true);
        translate([sma_top_x,case_y/2,sma_height])
            rotate([90,0,0]) cylinder(h=15,d=sma_barrel_d,center=true);
        translate([display_offset_x+jst_x,-case_y/2+3,
                   rear_depth-jst_body_z/2-3])
            cube([jst_body_x,12,jst_body_z],center=true);
    }
}

module assembly() {
    color([0.08,0.1,0.12]) front_bezel();
    color([0.12,0.14,0.16]) translate([0,0,front_depth+host_stack_z+12])
        rotate([180,0,0]) rear_shell();
    host_keepout();
    hat_keepout();
    connector_keepouts();
}

module product_view() {
    color([0.05,0.07,0.09]) front_bezel();
    // Review-only screen artwork; not a separate display component.
    color([0.08,0.28,0.42])
        translate([display_offset_x, host_active_y/4, front_depth-0.05])
            cube([host_active_x,host_active_y/2,0.3],center=true);
    color([0.24,0.18,0.10])
        translate([display_offset_x,-host_active_y/4,front_depth-0.05])
            cube([host_active_x,host_active_y/2,0.3],center=true);
    color([0.25,0.75,0.72])
        translate([display_offset_x,0,front_depth+0.22])
            cube([30,1.2,0.3],center=true);
    for (y=button_y)
        color([0.22,0.26,0.29])
            translate([button_x,y,front_depth-0.05]) cylinder(h=1.2,d=6.4);
    color([0.95,0.65,0.18])
        translate([case_x/2-28,-case_y/2+4,front_depth+0.25])
            linear_extrude(0.3) text("CONCEPT CAD",size=3.0,halign="center");
}

if (part == "front") front_bezel();
else if (part == "rear") rear_shell();
else if (part == "product") product_view();
else if (part == "fit_check") { host_keepout(); hat_keepout(); connector_keepouts(); }
else assembly();
