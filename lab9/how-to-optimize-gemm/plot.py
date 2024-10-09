import matplotlib.pyplot as plt
plt.rcParams.update({'font.size': 15})

fig, ax = plt.subplots(figsize=(15, 10))
x = [8, 16, 32, 64, 128, 256, 512, 1024, 2048]
ijk = [0.3413333,0.4311579,0.3076808,0.4941451,0.3208371,0.3203562,0.2361149,0.2098076,0.06895994]
ikj = [0.3413333,0.4818824,0.5201270,0.5328130,0.5402942,0.6620059,0.6239971,0.6263835,0.6267700]
kij = [0.5120000,0.6301538,0.5160315,0.6561802,0.6345392,0.6517322,0.5690411,0.6013171,0.6242805]
kji = [0.5120000,0.5120000,0.6068148,0.5930860,0.4789112,0.4081899,0.3349251,0.2736344,0.08090880]
jik = [0.5120000,0.5461333,0.5698783,0.6168094,0.5500727,0.4968009,0.3961106,0.3705864,0.1009096]
jki = [0.5120000,0.5461333,0.5904144,0.6061133,0.5147017,0.4612103,0.3299095,0.2220228,0.07886396]
SIMD = [0.2560000,0.4096000,0.4398389,0.4706355,0.3944239,0.4707808,0.3804336,0.3179459,0.1175286]
block = [0.5120000,0.8192000,0.8192000,0.8552822,0.8232196,0.7933428,0.7700253,0.7621390,0.7851604]
blockT = [0.2560000,0.2730667,0.6898526,0.7919758,0.8008982,0.8063449,0.7779137,0.7371289,0.7947936]

plt.plot(x, ijk, label='ijk', color='b', linestyle='-', linewidth=3.0)
plt.plot(x, ikj, label='ikj', color='g', linestyle='-.', linewidth=3.0)
plt.plot(x, kij, label='kij', color='r', linestyle=':', linewidth=3.0)
plt.plot(x, kji, label='kji', color='c', linestyle='-.', linewidth=3.0)
plt.plot(x, jik, label='jik', color='m', linestyle='-', linewidth=3.0)
plt.plot(x, jki, label='jki', color='y', linestyle=':', linewidth=3.0)
plt.plot(x, SIMD, label='SIMD', color='gray', linestyle='-', linewidth=3.0)
plt.plot(x, block, label='block', color='darkorange', linestyle='-', linewidth=3.0)
plt.plot(x, blockT, label='blockT', color='darkorchid', linestyle='-.', linewidth=3.0)


plt.title('dgemm')
plt.xlabel('matrix size')
plt.ylabel('gflops')
plt.legend()

plt.savefig('result.png')
