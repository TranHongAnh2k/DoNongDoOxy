import React, { useEffect, useState } from 'react'
import firebaseConfig from '../../firebase/firebaseConfig'
import firebase from 'firebase'
import BG from '../../assets/img/icon/bg.jpg'
const app = firebase.initializeApp(firebaseConfig)
const dbA = app.database().ref().child('deveiceA')
const dbB = app.database().ref().child('deveiceB')

const BarChar = () => {
  const [deveiceA, setDeveiceA] = useState<any>()
  const [deveiceB, setDeveiceB] = useState<any>()
  useEffect(() => {
    dbA.on('value',(snap:any)=>{
      setDeveiceA(snap.val())
    })
    dbB.on('value',(snap:any)=>{
      setDeveiceB(snap.val())
    })

  }, [])
  console.log(deveiceA);
  
  return (
    <div>
      <img className='bg' src={BG} alt="" />
      <div  className="container">
      <div className="heart-beat">
        <h3>{deveiceA?.heart} bmp</h3>
        <h1>Nhịp tim</h1>
        <h3>{deveiceB?.heart} bmp</h3>
      </div>
      <div className="oxy">
        <h3>{deveiceA?.oxy} %</h3>
        <h1>Nồng độ Oxy</h1>
        <h3>{deveiceB?.oxy} %</h3>
      </div>
      </div>
    </div>
  )
}

export default BarChar