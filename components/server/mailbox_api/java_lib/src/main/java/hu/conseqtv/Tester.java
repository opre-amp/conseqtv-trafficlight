package hu.conseqtv;


public class Tester{
    private static TrafficLight trafficLight;

    public static void main(String[] args) {
        trafficLight = new TrafficLight();
        trafficLight.registerErrorHandler((s) -> {System.err.println(s); System.exit(-1);});

        try {
            testSetState();
            System.out.println("setState works as expected.");
            testSendSignal();
            System.out.println("sendSignal works as expected.");
            testSetTimeA();
            testSetTimeB();
            testSetTimeC();
            testSetTimeC_();
            testSetTimeD();
            testSetTimeE();
            testSetTimeF();
            testSetTimeG();
            testSetTimeH();
        } catch (InterruptedException e) {
            e.printStackTrace();
        }

    }

    private static void testSetState() throws InterruptedException {
        for(String state : TrafficLight.getStates()) {
            trafficLight.setState(state);
            Thread.sleep(10);
            if(trafficLight.getState().equals(state)) {
                System.out.println("Could not set state \"" + state + "\". Exiting.\n");
                System.exit(-1);
            }
        }
    }

    private static void testSendSignal() throws InterruptedException {
        trafficLight.setState(TrafficLight.getStates().get(0));
        trafficLight.sendSignal(TrafficLight.getSignals().get(0));
        Thread.sleep(10);
        if(!trafficLight.getState().equals(TrafficLight.getStates().get(1))) {
            System.out.println("Could not send signal \"" + TrafficLight.getSignals().get(0) + "\". Exiting.\n");
            System.exit(-1);
        }
        trafficLight.sendSignal(TrafficLight.getSignals().get(2));
        Thread.sleep(10);
        if(!trafficLight.getState().equals(TrafficLight.getStates().get(2))) {
            System.out.println("Could not send signal \"" + TrafficLight.getSignals().get(2) + "\". Exiting.\n");
            System.exit(-1);
        }
        trafficLight.sendSignal(TrafficLight.getSignals().get(1));
        Thread.sleep(10);
        if(!trafficLight.getState().equals(TrafficLight.getStates().get(0))) {
            System.out.println("Could not send signal \"" + TrafficLight.getSignals().get(1) + "\". Exiting.\n");
            System.exit(-1);
        }
    }

    private static final Integer[] timeTestData = {100, 1000, 3000, 5000, 10000};

    private static void testSetTimeA(){
        long start, stop;
        for(int i : timeTestData) {
            trafficLight.setTimeA(i);
            start =  System.currentTimeMillis();
            trafficLight.setState(TrafficLight.getStates().get(2));
            while(!trafficLight.getState().equals(TrafficLight.getStates().get(3))) {}
            stop = System.currentTimeMillis();
            if(Math.abs(stop - start - i) > 10) {
                System.out.println("Could not set time A to " + i + "ms, it is " + (stop - start) + "ms.  Exiting.\n");
                System.exit(-1);
            }
        }
    }
    private static void  testSetTimeB(){
        long start, stop;
        for(int i : timeTestData) {
            trafficLight.setTimeB(i);
            start =  System.currentTimeMillis();
            trafficLight.setState(TrafficLight.getStates().get(3));
            while(!trafficLight.getState().equals(TrafficLight.getStates().get(4))) {}
            stop = System.currentTimeMillis();
            if(Math.abs(stop - start - i) > 10) {
                System.out.println("Could not set time B to " + i + "ms, it is " + (stop - start) + "ms.  Exiting.");
                System.exit(-1);
            }
        }
    }
    private static void  testSetTimeC(){
        long start, stop;
        for(int i : timeTestData) {
            trafficLight.setTimeC(i);
            start =  System.currentTimeMillis();
            trafficLight.setState(TrafficLight.getStates().get(4));
            while(!trafficLight.getState().equals(TrafficLight.getStates().get(6))) {}
            stop = System.currentTimeMillis();
            if(Math.abs(stop - start - i) > 10) {
                System.out.println("Could not set time C to " + i + "ms, it is " + (stop - start) + "ms.  Exiting.");
                System.exit(-1);
            }
        }
    }
    private static void  testSetTimeC_(){
        long start, stop;
        for(int i : timeTestData) {
            trafficLight.setTimeC_(i);
            start =  System.currentTimeMillis();
            trafficLight.setState(TrafficLight.getStates().get(5));
            while(!trafficLight.getState().equals(TrafficLight.getStates().get(6))) {}
            stop = System.currentTimeMillis();
            if(Math.abs(stop - start - i) > 10) {
                System.out.println("Could not set time C_ to " + i + "ms, it is " + (stop - start) + "ms.  Exiting.");
                System.exit(-1);
            }
        }
    }
    private static void  testSetTimeD(){
        long start, stop;
        for(int i : timeTestData) {
            trafficLight.setTimeD(i);
            start =  System.currentTimeMillis();
            trafficLight.setState(TrafficLight.getStates().get(6));
            while(!trafficLight.getState().equals(TrafficLight.getStates().get(7))) {}
            stop = System.currentTimeMillis();
            if(Math.abs(stop - start - i) > 10) {
                System.out.println("Could not set time D to " + i + "ms, it is " + (stop - start) + "ms.  Exiting.");
                System.exit(-1);
            }
        }
    }
    private static void  testSetTimeE(){
        long start, stop;
        for(int i : timeTestData) {
            trafficLight.setTimeE(i);
            start =  System.currentTimeMillis();
            trafficLight.setState(TrafficLight.getStates().get(7));
            while(!trafficLight.getState().equals(TrafficLight.getStates().get(8))) {}
            stop = System.currentTimeMillis();
            if(Math.abs(stop - start - i) > 10) {
                System.out.println("Could not set time E to " + i + "ms, it is " + (stop - start) + "ms.  Exiting.");
                System.exit(-1);
            }
        }
    }
    private static void  testSetTimeF(){
        long start, stop;
        for(int i : timeTestData) {
            trafficLight.setTimeF(i);
            start =  System.currentTimeMillis();
            trafficLight.setState(TrafficLight.getStates().get(8));
            while(!trafficLight.getState().equals(TrafficLight.getStates().get(9))) {}
            stop = System.currentTimeMillis();
            if(Math.abs(stop - start - i) > 10) {
                System.out.println("Could not set time F to " + i + "ms, it is " + (stop - start) + "ms.  Exiting.");
                System.exit(-1);
            }
        }
    }
    private static void  testSetTimeG(){
        long start, stop;
        for(int i : timeTestData) {
            trafficLight.setTimeG(i);
            start =  System.currentTimeMillis();
            trafficLight.setState(TrafficLight.getStates().get(9));
            while(!trafficLight.getState().equals(TrafficLight.getStates().get(10))) {}
            stop = System.currentTimeMillis();
            if(Math.abs(stop - start - i) > 10) {
                System.out.println("Could not set time G to " + i + "ms, it is " + (stop - start) + "ms.  Exiting.");
                System.exit(-1);
            }
        }
    }
    private static void  testSetTimeH(){
        long start, stop;
        for(int i : timeTestData) {
            trafficLight.setTimeH(i);
            start =  System.currentTimeMillis();
            trafficLight.setState(TrafficLight.getStates().get(10));
            while(!trafficLight.getState().equals(TrafficLight.getStates().get(2))) {}
            stop = System.currentTimeMillis();
            if(Math.abs(stop - start - i) > 10) {
                System.out.println("Could not set time H to " + i + "ms, it is " + (stop - start) + "ms.  Exiting.");
                System.exit(-1);
            }
        }
    }
}