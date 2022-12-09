path = $1
pattern = $2
if [ -z "$path" ] || [ -z "$pattern" ]
then 
    echo 'Help: path pattern'
    exit 1
else 
    ls -a "$path" | readlink -f $(grep "$pattern") > result.txt
    echo 'Your result in output.txt'
fi