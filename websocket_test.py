import asyncio
import websockets

# 接收客户端消息并处理，这里只是简单把客户端发来的返回回去
async def send_msg(websocket):
    while True:
        print("sending...")
        response_text = "{\
                    'messageId': 'CBAQAiAAMAE=', \
                    'payload': 'AAAA', \
                    'properties':{ \
                        'content': '日出江花红胜火，春来江水绿如蓝。', \
                        'id': 'UMwXbJ7xE5xC4rJl8VZ8ZcUZ5n+qWs5I',  \
                        'sender': 'user_{wx_openid}', \
                        'permission': '1', \
                        'color': 'undefined', \
                        'size': '16pt',\
                        'pos': 'rightleft'\
                    },\
                    'publishTime': '2021-11-21T10:56:25.798Z',\
                    'redeliveryCount': 0\
                }"
        #response_text="{\"size\":10}"
        await websocket.send(response_text.encode("utf-8"))
        await asyncio.sleep(1)

# 服务器端主逻辑
# websocket和path是该函数被回调时自动传过来的，不需要自己传
async def main_logic(websocket, path):
    print("connected")
    await send_msg(websocket)

# 把ip换成自己本地的ip
start_server = websockets.serve(main_logic, '127.0.0.1', 8000)
# 如果要给被回调的main_logic传递自定义参数，可使用以下形式
# 一、修改回调形式
# import functools
# start_server = websockets.serve(functools.partial(main_logic, other_param="test_value"), '10.10.6.91', 5678)
# 修改被回调函数定义，增加相应参数
# async def main_logic(websocket, path, other_param)

asyncio.get_event_loop().run_until_complete(start_server)
asyncio.get_event_loop().run_forever()