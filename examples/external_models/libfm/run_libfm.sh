export rundir=$(pwd)
export executable=$rundir/libfm_with_BPR/bin/libFM
export threads=10
export calculate_lists_per_thread=50

export dim=20
export iter=20
export neg_sample=30
export learn_rate=0.05
export regular=0.001
export top_k=100

#
# training all models
#
pushd batches
for d in */; do
	echo training $d
	pushd $d
	mkdir -p run

	$executable \
		-task r -train train.libfm -test test.libfm \
		-dim "1,1,$dim" -method bpr -relation FIXED,SAMPLED -threads $threads \
		-iter $iter --neg_sample $neg_sample \
		-learn_rate $learn_rate -regular $regular \
		--out_vectors run/trained_model

	popd
done
popd

#
# calculating ranking lists
#
calculate_for_ids(){
	mkdir -p run/ranks
	$executable \
		-task r -train train.libfm -test test.libfm \
		-dim "1,1,$dim" -method bpr -relation FIXED,SAMPLED -threads 1 \
		-iter 0 \
		--top_k $top_k --list_id_output $(echo $@ | sed 's/ /,/g') \
		--out_ranked_list_dir run/ranks \
		--resume_state run/trained_model > /dev/null
}
export -f calculate_for_ids

pushd batches
for d in $(echo */ | tr " " "\n"); do
	pushd $d
	echo calculating toplists for $d
	cat test_users.txt | \
		xargs -d' ' -n $threads echo | \
		xargs -n$calculate_lists_per_thread -P $threads \
			bash -c 'calculate_for_ids "$@"' _

	> run/ranks.dat
	for f in run/ranks/*.dat; do
		fb=$(basename $f)
		fbt=${fb%.dat}
		# user position item
		awk "{print $fbt,\$1,NR}" $f
	done >> run/ranks.dat

	popd
done

export num=$(ls batch_*_prepared/run/ranks.dat | wc -l)
for i in $(seq 1 $num); do
	cp batch_${i}_prepared/run/ranks.dat batch_${i}_predictions.dat
done
popd