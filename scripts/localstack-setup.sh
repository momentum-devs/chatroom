#!/bin/bash

echo "CREATING S3 BUCKETS"

awslocal s3 mb s3://videos
awslocal s3 mb s3://resized-videos

echo "S3 BUCKETS CREATED"

awslocal s3 ls

echo "CREATING S3 OBJECTS"

awslocal s3 cp /usr/share/app/resources/sample_file1.mp4 s3://videos/sample_file1.mp4

awslocal s3 ls s3://videos --recursive --human-readable --summarize

echo "S3 OBJECTS CREATED"

