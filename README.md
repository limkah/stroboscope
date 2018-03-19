# Stroboscope
Basic stroboscope implementation for measuring shaft/pulley RPM.

Prototype implementation is with P-channel power MOSFET driving a 12V high power LED as a high-side switch. Switching is done with a smaller N-channel MOSFET from the logic pin of a microcontroller (in this case an Arduino for the initial setup and testing). Display is going to be a 2x16 character display, since that's what I have in the parts bin, may switch to a 7 segment if I have to order parts.

Status:

- Basic board layout added for planning purposes.
- Added Rev 0 of firmware (functioning prototype).