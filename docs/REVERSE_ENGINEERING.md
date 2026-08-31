# Safe T8L reverse engineering

No step below authorizes destructive modification. Stop before soldering to an
unknown or RF-related node and ask the owner.

## Preparation

1. Remove cells and USB. Wait, then confirm all rails are below 100 mV.
2. Photograph enclosure disassembly, cable orientation, screws, spacers, antenna
   routing, PCB sides, and connector markings. Include a metric scale and label
   every photograph.
3. Use ESD controls. Never power an RF transmitter without its intended antenna/
   load configuration. Do not probe RF matching components with powered tools.
4. Record multimeter/scope/logic-analyzer models and probe ground arrangement.

## Geometry checklist

Measure PCB outline/thickness, hole diameters and XY centers from two datums,
connector bodies/extraction directions, gimbal outer/bearing/stick/fastener
envelopes, switch centers and travel, antenna keepout, cell bays/contacts, shell
bosses, USB opening, and all cable bend radii. Provide uncertainty and photos;
do not round inferred dimensions into facts.

## Power-off mapping

Use continuity/diode/resistance modes with current-limited instruments:

1. Establish ground from battery negative, USB shield, and large ground pours.
2. Identify direct battery nodes and fuses/protection only by continuity and
   component markings. A low resistance is not automatically a power rail.
3. Measure battery-contact topology with no cells. Verify again with individually
   current-limited dummy cell sources before concluding series/parallel.
4. For each unknown connector, map ground and passive connectivity only. Leave
   all other pins `UNKNOWN`; do not inject a pull-up, logic pattern, or supply.

## Powered observation

Use a current-limited bench source at the documented input range. Begin below the
observed normal current limit and watch for unexpected draw/heat.

1. Measure DC rails with a 10 MΩ DMM, then ripple with a short spring-ground scope
   probe. Never use a long grounded clip near switch/RF nodes.
2. Observe candidate digital nodes with a high-impedance probe first. Record idle,
   boot, stick movement, bind, Wi-Fi, and RF power changes.
3. For serial candidates, measure voltage range and idle polarity before attaching
   a logic analyzer. Use a high-value series resistor and never drive the line.
4. Decode only after raw captures establish baud/framing; preserve captures and
   note firmware version. CRSF-like bytes do not prove electrical ownership.
5. Test USB descriptors/modes externally first. Record whether RF link and channel
   updates continue, but do not fly during an unproven mode.

## Stop conditions

Stop on unexpected heating, >10% current change without explanation, RF link
degradation, a rail outside its observed normal range, uncertain common ground,
or any required trace cut/component removal. Destructive access requires explicit
owner approval and a recovery plan.

