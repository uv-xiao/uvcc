#!/usr/bin/sh

cp ../uvcc/src/frontend/*.cpp ../uvcc/src/frontend/*.y ../uvcc/src/frontend/*.l ./
rm -rf parser*pp lex*
git add .
git commit -m "debug frontend"
git push origin frontend
