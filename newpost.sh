#!/usr/bin/env bash

DATE=$(date +"%F")
POSTS="_posts"

TITLE=$1
if [ -z "$TITLE" ]; then
  read -p 'Title: ' TITLE
fi

# replace ' ' with '-'
TITLENS=$(echo "$TITLE" | sed 's/ /-/g')

FILE=$POSTS/$DATE-$TITLENS.md

cat >$FILE <<EOM
---
layout: post
title:  "$TITLE"
date:   $DATE
categories: news
---


EOM

mate $FILE
