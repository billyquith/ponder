#!/usr/bin/env bash

exec bundle exec jekyll serve --baseurl ""

echo 'http://127.0.0.1:4000/' | pbcopy
