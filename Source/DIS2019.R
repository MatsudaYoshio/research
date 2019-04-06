library(exactRankTests)

all.data <- read.csv("experiment_data.csv")

# それぞれの手法のアンケートデータだけ取り出す
opt.data <- all.data[13:19]
fix.data <- all.data[20:ncol(all.data)]

## 正規性の検定
# 全アンケートデータ一つのベクトルにする
all.vec <- append(as.vector(t(opt.data)), as.vector(t(fix.data)))

# シャピロ・ウィルク検定
shapiro.test(x=all.vec)

# 各被験者のそれぞれの手法の中央値を計算
opt.median <- apply(opt.data, 1, median)
fix.median <- apply(fix.data, 1, median)

# 中央値に対してウィルコクソンの符号順位検定
wilcox.exact(x=opt.median,y=fix.median,paired=T)

# comfortability
wilcox.exact(x=t(opt.data[1]),y=t(fix.data[1]),paired=T)

# fun
wilcox.exact(x=t(opt.data[2]),y=t(fix.data[2]),paired=T)

# physical fatigue
wilcox.exact(x=t(opt.data[3]),y=t(fix.data[3]),paired=T)

# mental fatigue
wilcox.exact(x=t(opt.data[4]),y=t(fix.data[4]),paired=T)

# ease of use
wilcox.exact(x=t(opt.data[5]),y=t(fix.data[5]),paired=T)

# efficiency
wilcox.exact(x=t(opt.data[6]),y=t(fix.data[6]),paired=T)

# interference
wilcox.exact(x=t(opt.data[7]),y=t(fix.data[7]),paired=T)