while :
do
    echo -n "Enter the path to the FOLDER: "
    read VAR1

    if [[ ! -d $VAR1 ]];
        then
            echo "This directory does not exist!"
        else 
            break
    fi
done

while :
do
    echo -n "Enter FILE name pattern: "
    read VAR2

    if [[ -z "$VAR2" ]];
        then
            echo "This enter is empty!"
        else 
            break
    fi
done

echo 'Enter the number of the log file in which the path will be saved:'
read numberLog
ls -a $VAR1 | readlink -f $(grep $VAR2) > "./info${numberLog}.txt"
echo "Information was written in 'info${numberLog}.txt"