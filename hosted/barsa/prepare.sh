#!/bin/bash

# remove old zip
rm src/static/do_not_look_at_me.zip

# dir for zipping
mkdir src_tmp

# compile and push CryptoLib
python -m py_compile src/CryptoLib.py
mv src/CryptoLib.pyc src_tmp/

# push all stuff, remove flag file
cp -rf src/static src/templates src/app.py src_tmp/
rm  src_tmp/static/images/fc96f22d5230d38d0a81d2efc86ef5c051ecb811b4b601e2bb18909f0064fb92.gif

# remove flag filename from app.py
cp src_tmp/app.py src_tmp/app_tmp.py
sed 's/^FLAG = .*/FLAG = "\/images\/login_as_admin_to_get_it"/g' src_tmp/app_tmp.py > src_tmp/app.py
rm src_tmp/app_tmp.py

# push zip decryption pass
echo '~<O>~' > src_tmp/the_password_cant_be_and_is_not_whisky.txt

# zip pushed files
zip -r -e -P whisky do_not_look_at_me.zip src_tmp

# move zip to public folder
mv do_not_look_at_me.zip src/static/

rm -rf src_tmp