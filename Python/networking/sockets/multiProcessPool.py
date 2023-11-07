import asyncio
from aiohttp import request
from aiomultiprocess import Pool

async def get(url):
    async with request("GET", url) as response:
        return await response.text("utf-8")

async def main():
    urls = ["https://noswap.com", ...]
    async with Pool() as pool:
        async for result in pool.map(get, urls):
            ...  # process result
            
if __name__ == '__main__':
    # Python 3.7
    asyncio.run(main())
    
    # Python 3.6
    # loop = asyncio.get_event_loop()
    # loop.run_until_complete(main())