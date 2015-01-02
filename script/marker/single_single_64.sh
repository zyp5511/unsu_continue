#!/bin/bash
pushd /home/lichao/scratch/pami
rm -r motor_res_64 motor_face_64 motor_car_64 motor_airplane_64
mkdir motor_res_64 motor_face_64 motor_car_64 motor_airplane_64
popd
#ruby process_rect.rb if draw_group_quality /home/lichao/scratch/pami/data/car_test/ /home/lichao/scratch/pami/motor_car_64/ /home/lichao/scratch/pami/motor_train_car_64.txt /home/lichao/scratch/pami/motor_train_kmeans_800_64.txt nil /home/lichao/scratch/pami/motor_train_net2_raw_64.txt /home/lichao/scratch/pami/motor_train_net2_el_64.txt /home/lichao/scratch/pami/motor_train_global_summary_64.txt /home/lichao/scratch/pami/motor_train_voro_centers_64.txt|tee car_64.txt
#ruby process_rect.rb if draw_group_quality /home/lichao/scratch/pami/data/airplane_test/ /home/lichao/scratch/pami/motor_airplane_64/ /home/lichao/scratch/pami/motor_train_airplane_64.txt /home/lichao/scratch/pami/motor_train_kmeans_800_64.txt nil /home/lichao/scratch/pami/motor_train_net2_raw_64.txt /home/lichao/scratch/pami/motor_train_net2_el_64.txt /home/lichao/scratch/pami/motor_train_global_summary_64.txt /home/lichao/scratch/pami/motor_train_voro_centers_64.txt|tee airplane_64.txt
ruby process_rect.rb if draw_group_quality /home/lichao/scratch/pami/data/face_test/ /home/lichao/scratch/pami/motor_face_64/ /home/lichao/scratch/pami/motor_train_face_64.txt /home/lichao/scratch/pami/motor_train_kmeans_800_64.txt nil /home/lichao/scratch/pami/motor_train_net2_raw_64.txt /home/lichao/scratch/pami/motor_train_net2_el_64.txt /home/lichao/scratch/pami/motor_train_global_summary_64.txt /home/lichao/scratch/pami/motor_train_voro_centers_64.txt|tee face_64.txt
ruby process_rect.rb if draw_group_quality /home/lichao/scratch/pami/data/motorbike_test/ /home/lichao/scratch/pami/motor_res_64/ /home/lichao/scratch/pami/motor_train_test_res_64.txt /home/lichao/scratch/pami/motor_train_kmeans_800_64.txt nil /home/lichao/scratch/pami/motor_train_net2_raw_64.txt /home/lichao/scratch/pami/motor_train_net2_el_64.txt /home/lichao/scratch/pami/motor_train_global_summary_64.txt /home/lichao/scratch/pami/motor_train_voro_centers_64.txt|tee motorbike_64.txt
