# 実行時間はハードコードしている
exec_times = [0.000001, 0.000016, 0.000253, 0.001277, 0.09562, 0.107627, 01.401367, 19.157938]

base_data_num = 10
target_data_num = 100

for i in range(1, 8):
    ratio = exec_times[i] / exec_times[i-1]
    print("データ数を{}から{}まで{}倍したとき、実行時間は{}倍".format(base_data_num, target_data_num, target_data_num/base_data_num, ratio))

    base_data_num = target_data_num
    target_data_num = target_data_num*10
