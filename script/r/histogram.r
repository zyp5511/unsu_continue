
# Read Data ---------------------------------------------------------------
scratch = 'C:/Users/Lichao/scratch/'
ns = c('car','face','motorbike','airplane')
library(ggplot2)

for(m in ns){
  dats = list()
  for(n in ns){
    dats[[n]] = read.table(paste(scratch,m,'/', 'test_',n,'_res_64.txt',sep=''))
    names(dats[[n]])=c('node','Distance')
    dats[[n]]$Category = n
  }
  alltable = do.call('rbind',dats)
  ggplot(alltable[,c('Distance','Category')], aes(Distance, fill = Category)) + geom_density(alpha = 0.2)+ labs(title = paste(m,'Detector'))
  ggsave(file=paste(m,"hist.pdf",sep='_'), width=8, height=6)
}
