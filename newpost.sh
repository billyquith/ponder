#!/usr/bin/env bash

DATE=$(date +"%F")
POSTS="_posts"

TITLE=$1
if [ -z "$TITLE" ]; then
  read -p 'Title: ' TITLE
fi

# replace ' ' with '-'
TITLE=$(echo "$TITLE" | sed 's/ /-/g')

FILE=$POSTS/$DATE-$TITLE.md

cat >$FILE <<EOM
---
layout: post
title:  "$TITLE"
date:   $DATE
categories: news
---


EOM

mate $FILE
