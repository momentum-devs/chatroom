#!/bin/bash

echo "CREATING S3 BUCKETS"

awslocal s3 mb s3://avatars

awslocal s3 ls

echo "S3 BUCKETS CREATED"

echo "CREATING S3 OBJECTS"

awslocal s3 cp /usr/share/app/resources/example_avatar.jpg s3://avatars/example_avatar.jpg

awslocal s3 ls s3://avatars --recursive --human-readable --summarize

echo "S3 OBJECTS CREATED"
