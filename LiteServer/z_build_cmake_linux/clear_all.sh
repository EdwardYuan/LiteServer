build_list=(
'common'
'msgdefine'
'gateway'
'testlib'
)

for dir in ${build_list[*]}
do
	echo $dir
	cd $dir
	rm -rf build
	cd ..
done