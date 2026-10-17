# ex23 Pedestrian button on the traffic light

Extend ex22 with a button on pin 2 (INPUT_PULLUP, debounced) that generates `Event::PED_REQUEST`.
Add two rows to the table: GREEN + PED_REQUEST -> YELLOW (shorten the green phase), RED + PED_REQUEST -> GREEN (pedestrian has finished crossing).
A request during YELLOW is ignored (no row). The engine must not change.

Checked: press at 7 s (during GREEN, which started at 5 s) -> yellow starts at 7 s; yellow lasts 3 s; red from 10 s; press at 12 s -> green at 12 s; press at 17.5 s (yellow) ignored.
