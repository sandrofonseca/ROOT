#! /bin/sh

SVN2CL=build/unix/svn2cl.sh

echo ""
echo "Generating README/ChangeLog from SVN logs..."
echo ""

# Generate ChangeLog from version v5-12-00 till now
$SVN2CL -f README/ChangeLog -r HEAD:15807

exit 0
