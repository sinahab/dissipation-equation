
rm(list=ls())

require(data.table)
require(ggplot2)

n.grid <- 1000
file <- "bar-implicit_deltaxE-3_deltatE-3"
plot.title <- "implicit solution to heat equation\ndelta_x = 1E-3\ndelta_t = 1E-3"

times <- data.frame(times=NA)
x.positions <- seq(from=0,to=n.grid-1,by=1)
  
file.handle <- file(paste("/Users/sinahab/Dropbox/ubuntu_comp/",file,sep=""), open="r")
text <- readLines(file.handle)
num.lines <- length(text)

for(i in 3:num.lines) {
  # matching for times
  if (substring(text[i],4,7) == "time"){
    times <- rbind(times, substring(text[i],9,nchar(text[i])-1))
  }
}
times <- times[-1,]
times <- as.numeric(times)

# initiallizing the data.frame with the x temperatures
# x.temperatures <- as.data.frame(matrix(nrow=n.grid,ncol=length(times)))
num.x <- 1
num.t <- 1

temperatures <- data.table( x.position = rep(x.positions,length(times)),
                            time = rep(times,n.grid) )
setkey(temperatures, x.position, time)


for (i in 3:num.lines) {
  # matching for x positions
  if(!is.na(as.numeric(substring(text[i],1,1)))){
    split <- strsplit(text[i],"\t")
        
   #  x.temperatures[num.x,num.t] <- substring(split[[1]][2],2,nchar(split[[1]][2])-1)
    temperatures[time==times[num.t] & x.position==x.positions[num.x],
                 temperature:=as.numeric(substring(split[[1]][2],2,nchar(split[[1]][2])-1))]
    
    num.x <- num.x+1
  }
  if (num.x==n.grid+1){
    num.x <- 1
    num.t <- num.t+1
  } 
}

close(file.handle)

png(paste(paste("/Users/sinahab/Dropbox/ubuntu_comp/",file,sep=""),".png",sep=""), width=1000, height=1000)

ggplot(data=temperatures) + 
  geom_line(aes(x=x.position, y=temperature, colour=as.factor(time))) +
  ggtitle(plot.title)  

dev.off()

