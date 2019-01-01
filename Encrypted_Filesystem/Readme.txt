
Westley Kirkham
Systems 1
April 25, 2018


	This encrypted file system was run using python 3.6.3 through the PyCharm IDE. to create the encrypted file system environment type: python encfsStarterCode.py encryptedfolder/ decryptedfolder. The folders may or may not be ones that already exist. If they are, the encryptedfolder, at the very least, must have been created using this same system or its files will be undecipherable and you will not be permitted to open them.
	Once the program begins running you'll be prompted to enter your password. If you are making a new file system you may use any password you like. If you are using the program to decrypt files in a folder you've already created the password must be correct or you will not be permitted to open or view the files, as mentioned before.
	Once you enter your password FUSE reports will begin steadily streaming through your terminal. In order to utilize the filesystem you'll need to open a new terminal and navigate to the location of your encryptedfolder and/or decryptedfolder.
	encfsStarterCode.py was tested on and performs typical bash commands. (e.g. ls, echo, cat) Any actions performed in the decryptedfolder directory will be mirrored in the encryptedfolder directory. Only, if you open the files in the encryptedfolder you'll find them encrypted. Files you open in the decryptedfolder will be plain text.


examples:


######user:location$ ls decryptedfolder/
hello test
######user:location$ ls encryptedfolder/
hello test
######user:location$ cat decryptedfolder/hello
hello
######user:location$ cat encryptedfolder/hello
?L~??
?#~?i???tgAAAAABa4TZIehXi7AubaC0ep8arjk_slsxKzPov0-				taESSwWKWU0gHulnGMvvsG2EScIOKo6kof3Wm1OoSer7sGOIpvKHpyjw==


Folders and files may be created through the mkdir and echo commands:


######user:location$ mkdir decryptedfolder/newdir
######user:location$ ls encryptedfolder/   (or decryptedfolder/. They should be the same)
hello test newdir
######user:location$ echo "testingnewdir" > decryptedfolder/newdir/newdirtest
######user:location$ ls decryptedfolder/newdir/
newdirtest
######user:location$ cat decryptedfolder/newdir/newdirtest
testingnewdir


The above operations were sufficient to test that open, create, read, write and release were functioning properly. To test truncate (shrink or grow a file) I echoed smaller and larger strings to files:


######user:location$ echo "short" > decryptedfolder/newdir/newdirtest
######user:location$ cat decryptedfolder/newdir/newdirtest
short
######user:location$ echo "longish test" > decryptedfolder/newdir/newdirtest
######user:location$ cat decryptedfolder/newdir/newdirtest
longish test


	When you are done interfacing with the encrypted file system you may stop the program by returning to the original terminal and typing CTRL + C. (if you're on Mac.) If it doesn't immediately quit or if it quits with an error you may have to manually unmount the filesystem. To do this type: umount decryptedfolder. 


	Thank you.



