echo -n "Enter the path to the FOLDER: "
read _var1

if [[ ! -d $_var1 ]];
then
    echo "This directory does not exist!"
    exit 0
fi

echo -n "Enter pattern: "
read _var2

if [[ -z "$_var2" ]];
then
    echo "This enter is empty!"
    exit 0
fi


ls -a $_var1 | readlink -f $(grep $_var2) > "./info.txt"
echo "Information was written in 'info.txt"