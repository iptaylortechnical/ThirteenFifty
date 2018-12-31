# Thirteen Fifty

A TFTP Client in C. RFC 1350 defines the TFTP protocol, hence thirteen fifty.

Supported RFC's: 

* 1350 - TFTP specification
* 2347 - Option specification
* 2348 - Blocksize option
* 7440 - Windowsize option


## RETURNED ERRORS:

These errors will be returned in the following format: {{err:\[number]}}\n
Areas in quotes, e.g. "option: '%s'\n" are output format specifiers to
make it easier to regex.

1. Bad port number
2. Bad blocksize
3. Bad windowsize
4. Bad timeout
5. Bad usage, missing target or filename
6. Unrecognized option in configuration "option: '%s'\n"
7. Could not get address info
8. Could not create socket with those DNS addresses
9. Unable to set socket timeout
10. Failed to send RRQ
11. Did not receive response to RRQ within timeout
12. Reached max retries
13. Error receiving packet
14. Error sending retry ACK
15. Error sending ACK
16. Error packet: error 0
17. Error packet: error 1
18. Error packet: error 2
19. Error packet: error 3
20. Error packet: error 4
21. Error packet: error 5
22. Error packet: error 6
23. Error packet: error 7
24. Error packet: error 8
25. Could not send OACK error packet
26. Found option without corresponding "value: '%s'\n"
27. Found blocksize in server options even though client did not request
28. Found windowsize in server options even though client did not request
29. Got unknown option in "OACK: '%s'\n"
30. Could not send ACK to OACK