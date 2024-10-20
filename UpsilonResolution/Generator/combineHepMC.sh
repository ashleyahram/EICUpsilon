#!/bin/bash

# Input files
file1="${1}"
file2="${2}"
file3="${3}"
output_file="${4}"

# Temporary file to hold combined events
temp_file="temp_combined.hepmc"

# Remove temporary files if they already exist
rm -f $temp_file $output_file

# Combine files
cat "$file1" "$file2" "$file3" > $temp_file

# Initialize counters
event_count=0
event_number=1

# Read the combined file line by line and update event numbers
while IFS= read -r line; do
    if [[ $line == E* ]]; then
        # Extract the rest of the line after the first number
        rest_of_line=$(echo "$line" | cut -d' ' -f3-)
        # Update the event number
        echo "E $event_number $rest_of_line" >> $output_file
        event_number=$((event_number + 1))
        event_count=$((event_count + 1))
    else
        echo "$line" >> $output_file
    fi
done < "$temp_file"

# Clean up
rm -f $temp_file

echo "Combined $event_count events into $output_file with updated event numbers"