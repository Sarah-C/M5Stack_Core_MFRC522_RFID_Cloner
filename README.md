# M5Stack Core: MFRC522 RFID Cloner
This is a Mifare Classic 1k RFID cloning program for the M5Stack Core, with RFID unit.

This program can read several types of tags, and also write to Mifare Classic 1K RFID tags that have the "UID Backdoor" that enables the cards User-Identity to be written to.
This is normally read only - so when buying them you need to ensure they have the ability for their UID to be written to.

The tags are a little coil with a small chip. They come in the form of Access-Cards, Keyring Fobs, Plastic discs, and even stickers!
 
**Instructions_____**                     
1: Place the tag near the RFID unit. A beep will sound and the screen will identify the RFID type (including Mifare Ultralight) and show the user identity as hex digit pairs.
Many access systems use just the UID! The storage area of the card is often not even used.                 
               
2: After reading at least 1 Mifare classic card, pressing button "A" (the left hand side one) on the M5Stack Core will switch to clone-mode.               
Every time a card is placed by/on the RFID unit, an attempt will be made to write the UID to the card.                   
A two tone high beep will sound indicating success. A lower tone buzz will indicate failure, and the screen will update showing the same.                
                  
https://shop.m5stack.com/products/rfid-sensor-unit                   
https://shop.m5stack.com/collections/stack-series/products/grey-development-core?variant=16804796006490                  
                  
![20210902_112912](https://user-images.githubusercontent.com/1586332/131832453-d9a7cc5b-46c7-4a41-aa61-940617f5f923.jpg)







