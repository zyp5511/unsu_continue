
# Read Data ---------------------------------------------------------------
scratch = 'C:/Users/Lichao/scratch/'
library(ggplot2)
suffix = '_res_64.txt'
ns = c('car','face','motorbike','airplane')



disthist <- function(prefix, suffix, m, ns ) {
  dats = list()
  for(n in ns){
    dats[[n]] = read.table(paste(prefix,n,suffix,sep=''))
    names(dats[[n]])=c('node','Distance')
    dats[[n]]$Category = n
  }
  alltable = do.call('rbind',dats)
  ggplot(alltable[,c('Distance','Category')], aes(Distance, fill = Category)) + geom_density(alpha = 0.2)+ labs(title = paste(m,'Detector'))
  ggsave(file=paste(prefix,m,"hist.pdf",sep='_'), width=8, height=6)
}


for(m in ns){
  prefix = paste(scratch,'separate',m, 'test_',sep='/')
  disthist(prefix, suffix,m,ns)
}

prefix = paste(scratch,'train','quad_train_',sep='/')
suffix = '_train_64.txt'
disthist(prefix, suffix,'Shared Dictionary',ns)