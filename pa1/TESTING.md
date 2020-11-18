# Testing

If you follow all directions provided while running the program, you will not run into any errors

program is started by: 

`./album (path to photos)`

If no path is provided, it will print "No path to photo" and then exit

If a correct path is provided it will print the amount of photos to be processed, display the first thumbnail, then ask the user:

`Would you like photo0.jpg to be rotated? y or n`

The first letter of the user input must either be "y" or "n". If another letter is detected, an invalid argument for rotation percentage is printed, and the photo will not be rotated, nor will you be able to provide a caption

If you choose yes, you will then be prompted with: 

`enter a rotation percentage. format: ###%`

The program will only read the first 3 digits entered. The input does not need to contain a '%' sign.

If no is chosen or you have entered the rotation percentage, it will then prompt the user for a caption. The program will read in 25 characters only.


After you insert a caption, you must press `enter` and then close the thumbnail to continue to the next picture where the loop will restart.


Once you have finished processing each photo, the program will call `create_page` function and an index.html file will be generated, using all the users photos and captions.
