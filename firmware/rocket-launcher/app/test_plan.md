# Rocket Launcher Test Plan

1. Connect PAD to -12 V
    * Fast red blinks on PAD
2. Connect LCO to power
    * Fast red blinks on LCO
3. Hold PAD in reset
    * Slow red blinks on LCO
4. Releast PAD from reset
    * Fast red blinks on LCO
5. Connect PAD to 11 V
    * Red fade
6. Connect PAD to 12 V
    * Yellow
7. Disconnect LCO from power
    * Fast red blinks on PAD
8. Connect LCO to power in reverse
    * Yellow
9. Connect charge to every channel, one at a time
    * Green for connected, yellow for disconnected channels
10. Flip switch 1 on
    * Rainbow, green, green, green
11. Press button
    * Charge 1 fires
    * Orange, green, green, green
12. Flip switch 2 on
    * Orange, rainbow, green, green
13. Release button
    * Orage, rainbow, green, green
14. Flip switch 1 off
    * Orange, rainbow, green, green
15. Press button
    * Orange, rainbow, green, green
16. Flip switch 2 off
    * Orange, green, green, green
17. Release button
    * Yellow, green, green, green
18. Flip switch 2 on
    * Yellow, rainbow, green, green
19. Flip switch 3 on
    * Yellow, rainbow, rainbow
20. Press button
    * Charges 2 and 3 fire
    * Yellow, orange, orange, green
21. Flip switch 3 off
    * Yellow, orange, orange, green
22. Release button
    * Yellow, orange, orange, green
23. Press button
    * Yellow, orange, orange, green
24. Release button
    * Yellow, orange, orange, green
25. Flip switch 2 off
    * Yellow, yellow, yellow, green
26. Flip switch 4 on
    * Yellow, yellow, yellow, rainbow
27. Press button
    * Charge 4 fires
    * Yellow, yellow, yellow, orange
28. Release button
    * Yellow, yellow, yellow, orange
29. Flip switch 4 off
    * Yellow
30. Connect nichrome to channel 1
    * Green, yellow, yellow, yellow
31. Flip switch 1 on
    * Rainbow, yellow, yellow, yellow
32. Press button
    * Nichrome glows momentarily
    * Orange, yellow, yellow, yellow
33. Release button
    * Orange, yellow, yellow, yellow
34. Press button
    * Orange, yellow, yellow, yellow
35. Flip switch 1 off
    * Orange, yellow, yellow, yellow
36. Release button
    * Green, yellow, yellow, yellow
37. Press button
    * Green, yellow, yellow, yellow
38. Release button
    * Green, yellow, yellow, yellow
39. Disconnect nichrome
    * Yellow
40. Connect charge to every channel, one at a time
    * Green for connected, yellow for disconnected channels
41. Flip every switch on, one at a time
    * Rainbow for flipped, green for unflipped channels
42. Press button
    * All charges fire
    * Orange
43. Flip every switch to off
    * Orange
44. Release button
    * Yellow
