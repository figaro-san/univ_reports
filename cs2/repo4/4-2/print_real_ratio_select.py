# 後に変更(select用に)
exec_times = [0.000001, 0.000009, 0.000050, 0.000600, 0.002116, 0.025470, 0.162877, 2.016854]

base_data_num = 10
target_data_num = 100

for i in range(1, 8):
    ratio = exec_times[i] / exec_times[i-1]
    print("データ数を{}から{}まで{}倍したとき、実行時間は{}倍".format(base_data_num, target_data_num, target_data_num/base_data_num, ratio))

    base_data_num = target_data_num
    target_data_num = target_data_num*10
