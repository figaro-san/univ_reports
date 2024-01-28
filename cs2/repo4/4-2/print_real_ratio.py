# 実行時間はハードコードしている
exec_times = [0.000002, 0.000042, 0.000603, 0.004684, 0.012677, 0.070718, 0.802539, 10.727278]

base_data_num = 10
target_data_num = 100

for i in range(1, 8):
    ratio = exec_times[i] / exec_times[i-1]
    print("データ数を{}から{}まで{}倍したとき、実行時間は{}倍".format(base_data_num, target_data_num, target_data_num/base_data_num, ratio))

    base_data_num = target_data_num
    target_data_num = target_data_num*10
