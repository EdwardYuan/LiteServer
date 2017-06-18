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
	mkdir build
	cd build
	cmake ../
	make
	cd ..
	cd ..
done
